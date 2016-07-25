//---------------------------------------------------------------------------
#ifndef ISinkH
#define ISinkH
//---------------------------------------------------------------------------
namespace Cashup
{
//---------------------------------------------------------------------------
template <typename T> class ISink
{
public:

	virtual void Start() {};
	virtual void Sink(const T& const value) = 0;
	virtual void End() {};
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif
