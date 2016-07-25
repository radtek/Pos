#define __IXMLNODE_IMPL2__ \
  virtual ULONG __stdcall AddRef() { \
    return TXMLNode::_AddRef(); \
  } \
  virtual ULONG __stdcall Release()  { \
    return TXMLNode::_Release(); \
  } \
  virtual OleVariant __fastcall GetAttribute(const System::UnicodeString AttrName){ \
    return TXMLNode::GetAttribute(AttrName); \
  } \
  virtual _di_IXMLNodeList __fastcall GetAttributeNodes(){ \
    return TXMLNode::GetAttributeNodes(); \
  } \
  virtual _di_IXMLNodeList __fastcall GetChildNodes(){ \
    return TXMLNode::GetChildNodes(); \
  } \
  virtual OleVariant __fastcall GetChildValue(const OleVariant &IndexOrName){ \
    return TXMLNode::GetChildValue(IndexOrName); \
  } \
  virtual _di_IXMLNodeCollection __fastcall GetCollection(){ \
    return TXMLNode::GetCollection(); \
  } \
  virtual Xmldom::_di_IDOMNode __fastcall GetDOMNode(){ \
    return TXMLNode::GetDOMNode(); \
  } \
  virtual bool __fastcall GetHasChildNodes(){ \
    return TXMLNode::GetHasChildNodes(); \
  } \
  virtual bool __fastcall GetIsTextElement(){ \
    return TXMLNode::GetIsTextElement(); \
  } \
  virtual System::UnicodeString __fastcall GetLocalName(){ \
    return TXMLNode::GetLocalName(); \
  } \
  virtual System::UnicodeString __fastcall GetNamespaceURI(){ \
    return TXMLNode::GetNamespaceURI(); \
  } \
  virtual System::UnicodeString __fastcall GetNodeName(){ \
    return TXMLNode::GetNodeName(); \
  } \
  virtual TNodeType __fastcall GetNodeType(){ \
    return TXMLNode::GetNodeType(); \
  } \
  virtual OleVariant __fastcall GetNodeValue(){ \
    return TXMLNode::GetNodeValue(); \
  } \
  virtual _di_IXMLDocument __fastcall GetOwnerDocument(){ \
    return TXMLNode::GetOwnerDocument(); \
  } \
  virtual _di_IXMLNode __fastcall GetParentNode(){ \
    return TXMLNode::GetParentNode(); \
  } \
  virtual System::UnicodeString __fastcall GetPrefix(){ \
    return TXMLNode::GetPrefix(); \
  } \
  virtual bool __fastcall GetReadOnly(){ \
    return TXMLNode::GetReadOnly(); \
  } \
  virtual System::UnicodeString __fastcall GetText(){ \
    return TXMLNode::GetText(); \
  } \
  virtual System::UnicodeString __fastcall GetXML(){ \
    return TXMLNode::GetXML(); \
  } \
  virtual void __fastcall SetAttribute(const System::UnicodeString AttrName, const OleVariant &Value){ \
    TXMLNode::SetAttribute(AttrName, Value); \
  } \
  virtual void __fastcall SetChildValue(const OleVariant &IndexOrName, const OleVariant &Value){ \
    TXMLNode::SetChildValue(IndexOrName, Value); \
  } \
  virtual void __fastcall SetNodeValue(const OleVariant &Value){ \
    TXMLNode::SetNodeValue(Value); \
  } \
  virtual void __fastcall SetReadOnly(const bool Value){ \
    TXMLNode::SetReadOnly(Value); \
  } \
  virtual void __fastcall SetText(const System::UnicodeString Value){ \
    TXMLNode::SetText(Value); \
  } \
  virtual _di_IXMLNode __fastcall AddChild(const System::UnicodeString TagName, int Index = 0xffffffff){ \
    return TXMLNode::AddChild(TagName, Index); \
  } \
  virtual _di_IXMLNode __fastcall AddChild(const System::UnicodeString TagName, const System::UnicodeString NamespaceURI, bool GenPrefix = false, int Index = 0xffffffff){ \
    return TXMLNode::AddChild(TagName, NamespaceURI, GenPrefix, Index); \
  } \
  virtual _di_IXMLNode __fastcall CloneNode(bool Deep){ \
    return TXMLNode::CloneNode(Deep); \
  } \
  virtual void __fastcall DeclareNamespace(const System::UnicodeString Prefix, const System::UnicodeString URI){ \
    TXMLNode::DeclareNamespace(Prefix, URI); \
  } \
  virtual System::UnicodeString __fastcall FindNamespaceURI(const System::UnicodeString TagOrPrefix){ \
    return TXMLNode::FindNamespaceURI(TagOrPrefix); \
  } \
  virtual _di_IXMLNode __fastcall FindNamespaceDecl(const System::UnicodeString NamespaceURI){ \
    return TXMLNode::FindNamespaceDecl(NamespaceURI); \
  } \
  virtual OleVariant __fastcall GetAttributeNS(const System::UnicodeString AttrName, const System::UnicodeString NamespaceURI){ \
    return TXMLNode::GetAttributeNS(AttrName, NamespaceURI); \
  } \
  virtual bool __fastcall HasAttribute(const System::UnicodeString Name){ \
    return TXMLNode::HasAttribute(Name); \
  } \
  virtual bool __fastcall HasAttribute(const System::UnicodeString Name, const System::UnicodeString NamespaceURI){ \
    return TXMLNode::HasAttribute(Name, NamespaceURI); \
  } \
  virtual _di_IXMLNode __fastcall NextSibling(){ \
    return TXMLNode::NextSibling(); \
  } \
  virtual void __fastcall Normalize(){ \
    return TXMLNode::Normalize(); \
  } \
  virtual _di_IXMLNode __fastcall PreviousSibling(){ \
    return TXMLNode::PreviousSibling(); \
  } \
  virtual void __fastcall Resync(){ \
    TXMLNode::Resync(); \
  } \
  virtual void __fastcall SetAttributeNS(const System::UnicodeString AttrName, const System::UnicodeString NamespaceURI, const OleVariant &Value){ \
    TXMLNode::SetAttributeNS(AttrName, NamespaceURI, Value); \
  } \
  virtual void __fastcall TransformNode(const _di_IXMLNode stylesheet, System::WideString &output){ \
    TXMLNode::TransformNode(stylesheet, output); \
  } \
  virtual void __fastcall TransformNode(const _di_IXMLNode stylesheet, const _di_IXMLDocument output){ \
    TXMLNode::TransformNode(stylesheet, output); \
  }

