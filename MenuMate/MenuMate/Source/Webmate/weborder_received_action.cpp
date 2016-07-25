#include "XMLOrdermm.h"
#include "string_utilities.hh"
#include "webmate_path_configuration.hh"
#include "webmate_order_processor.hh"
#include "weborder_received_action.hh"
#include "weborder_response.hh"

/*
 * The CreateDirectory[AW] functions don't create a directory path recursively
 * so we have to create each component of the input path ourselves if they
 * don't already exist.
 */
bool
create_directory(
  const std::wstring directory)
{
    std::vector<std::wstring> path_components = split_string(directory, L"\\");
    std::wstring last_path_component = L"";

    for (std::vector<std::wstring>::const_iterator i = path_components.begin();
           i != path_components.end(); ++i) {
        const std::wstring path_to_create = last_path_component + *i;

        CreateDirectoryW(path_to_create.c_str(), NULL);

        switch (GetLastError()) {
        case ERROR_SUCCESS:
        case ERROR_ALREADY_EXISTS:
            break;
        case ERROR_PATH_NOT_FOUND:
            return false;
        }

        last_path_component = path_to_create + L"\\";
    }

    return true;
}

bool
ensure_directories_are_created(
  const webmate_path_configuration_t &configuration)
{
    return create_directory(configuration.cache_directory())
           && create_directory(configuration.failed_directory())
           && create_directory(configuration.processed_directory());
}

int
find_weborder_xml_offset(
  TMemoryStream &input_stream)
{
    const char * const buffer =
      reinterpret_cast<char *>(input_stream.Memory);
    const char * const xml_position = strstr(buffer, "<?xml");

    return xml_position == NULL ? -1
                                : xml_position - buffer;
}

bool
seek_to_xml_offset(
  TMemoryStream &input_buffer)
{
    const int xml_offset = find_weborder_xml_offset(input_buffer);

    input_buffer.Position = (xml_offset != -1) * xml_offset;

    return xml_offset != -1;
}

std::auto_ptr<TXMLDocument>
create_xml_document_from_stream(
  TMemoryStream &input_buffer)
{
    try {
        std::auto_ptr<TXMLDocument> doc(new TXMLDocument(NULL));

        doc->LoadFromStream(&input_buffer);
        return doc;
    } catch (...) {
        return std::auto_ptr<TXMLDocument>(NULL);
    }
}

weborder_received_action_t::weborder_received_action_t(
  webmate_order_processor_t &order_processor,
  const webmate_path_configuration_t &configuration)
  : order_processor_(order_processor),
    configuration_(configuration)
{
    ensure_directories_are_created(configuration_);
}

bool
save_response_and_xml_document(
  const std::wstring &save_base_path,
  TXMLDocument &xml,
  weborder_response_t &response,
  std::wstring &weborder_saved_as)
{
    const UnicodeString time_saved =
      Now().FormatString(L"yyyymmddhhnnss");
    const UnicodeString base_path = save_base_path.c_str();
    const UnicodeString base_filename =
      base_path
      + L"\\"
      + UnicodeString(response.order_id().c_str()) + L"-" + time_saved;
    const UnicodeString order_filename = base_filename + L"-in.xml";
    const UnicodeString response_filename = base_filename + L"-out.xml";

    try {
        xml.Active = true;
        xml.SaveToFile(order_filename);
    } catch (...) {
        response.message(
          L"We couldn't write the order for later processing.");
        response.result(weborder_response_t::R_ERROR);
        weborder_saved_as = L"Weborder wasn't saved.";
        return false;
    }

    weborder_saved_as = order_filename.c_str();

    response.message(L"The order's been queued for processing.");
    response.result(weborder_response_t::R_SUCCESS);

    try {
        std::auto_ptr<TXMLDocument> response_xml(response);

        response_xml->Active = true;
        response_xml->SaveToFile(response_filename);
    } catch (...) {
        response.message(L"The order's been queued but we couldn't store "
                         L"a copy of this response.");
        response.result(weborder_response_t::R_WARNING);
    }

    return true;
}

bool
create_and_save_xml_document(
  const std::wstring &save_base_path,
  TMemoryStream &input_buffer,
  weborder_response_t &response,
  std::wstring &weborder_saved_as)
{
    /* Unfortunately, we need to keep the ``weborder'' about so that we can
     * retrieve the order's ID /and/ not lose access to the raw XML.
     *
     * This is because we use the XML binding to access the elements of the
     * web-order. When we instantiate the binding, the binding takes ownership
     * of the XML document. I.e. it'll release the XML document when the
     * _di_IXMLORDERType goes out of scope...
     */
    _di_IXMLORDERType weborder;
    std::auto_ptr<TXMLDocument> xml;

    if (seek_to_xml_offset(input_buffer) == false) {
        response.message(L"No XML could be found in the request.");
        response.order_id(L"Unknown");
        response.result(weborder_response_t::R_ERROR);
        return false;
    }

    xml = create_xml_document_from_stream(input_buffer);
    if (xml.get() == NULL) {
        response.message(L"The received XML is malformed.");
        response.order_id(L"Unknown");
        response.result(weborder_response_t::R_ERROR);
        return false;
    }

    weborder = GetORDER_DontTakeOwnership(xml.get());
    response.order_id(weborder->Get_GUID().c_str());

    return save_response_and_xml_document(save_base_path,
                                          *xml,
                                          response,
                                          weborder_saved_as);
}

/*
 * We can't use TXMLDocument's SaveToStream method here as it writes data
 * out in UTF16 regardless of what we say.
 */
void
write_response_to_buffer(
  const weborder_response_t &response,
  TMemoryStream &output_buffer)
{
    std::auto_ptr<TXMLDocument> response_xml(response);
    UTF8String raw_xml;

    response_xml->SaveToXML(raw_xml);
    output_buffer.Write(raw_xml.c_str(),
                        raw_xml.ElementSize() * raw_xml.Length());
}

bool
weborder_received_action_t::action(
  TMemoryStream &input_buffer,
  TMemoryStream &output_buffer)
{
    weborder_response_t response;
    std::wstring weborder_saved_as;

    /* The Co* calls are to initialize COM. This is required as Embarcadero's
     * XML functionality is a COM binding for a Delphi library. Be sure to
     * call once CoUninitialize() every time you call CoInitialize(...).
     *
     * Also note that calling these two functions isn't required before calling
     * any one function from Embarcadero's XML library. Rather, you need to
     * setup COM/teardown COM before and after using that library.
     */
    CoInitialize(NULL);
        if (ensure_directories_are_created(configuration_) == false) {
            response.result(weborder_response_t::R_ERROR);
            response.message(L"Internal server fault.");
        } else if (create_and_save_xml_document(
                     configuration_.cache_directory(),
                     input_buffer,
                     response,
                     weborder_saved_as) == true)
            order_processor_.queue_a_file_to_be_processed(weborder_saved_as);
        write_response_to_buffer(response, output_buffer);
    CoUninitialize();

    return response.result() != weborder_response_t::R_ERROR;
}

