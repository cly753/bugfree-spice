#ifndef _NOTALGOCPP_HPP_
#define _NOTALGOCPP_HPP_

struct NotAlgoCpp {
	virtual int process(unsigned char image[], int width, int height, int type) = 0;
};

NotAlgoCpp* getAlgo();

#endif

// Type                     Value
// ===============================
// TYPE_3BYTE_BGR           5
// TYPE_4BYTE_ABGR          6
// TYPE_4BYTE_ABGR_PRE      7
// TYPE_BYTE_BINARY         12
// TYPE_BYTE_GRAY           10
// TYPE_BYTE_INDEXED        13
// TYPE_CUSTOM              0
// TYPE_INT_ARGB            2
// TYPE_INT_ARGB_PRE        3
// TYPE_INT_BGR             4
// TYPE_INT_RGB             1
// TYPE_USHORT_555_RGB      9
// TYPE_USHORT_565_RGB      8
// TYPE_USHORT_GRAY         11