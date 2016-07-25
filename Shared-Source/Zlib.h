//---------------------------------------------------------------------------
#ifndef ZipLibH
#define ZipLibH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <memory>
//---------------------------------------------------------------------------
#define Z_BUFFER_SIZE 4096  // the size of the input and output buffers
//---------------------------------------------------------------------------
struct z_stream_s;
//---------------------------------------------------------------------------
namespace Zlib
{
//---------------------------------------------------------------------------
unsigned long Adler(char *Data, int Length);
unsigned long CRC(char *Data, int Length);  
//---------------------------------------------------------------------------
class TZlib
{
public:
	TZlib();

	int Compress(TStream *InStream, TStream *OutStream, int Level);
	int Decompress(TStream *InStream, TStream *OutStream);

private:
	std::auto_ptr<z_stream_s> ZStream;	// typedef'd to z_stream in zlib.h

	int				LoadInput(TStream *InStream);
	int				FlushOutput(TStream *OutStream);

	unsigned char	InputBuffer[Z_BUFFER_SIZE];
	unsigned char	OutputBuffer[Z_BUFFER_SIZE];
};
//---------------------------------------------------------------------------
} // namespace Zlib
//---------------------------------------------------------------------------
#endif
