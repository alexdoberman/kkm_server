#pragma once
#include <string>
#include <algorithm>
#include <stdexcept>

#include <QByteArray>
#include <QTextCodec>

#include "kkm_driver_def.h"

inline std::string ba2hex(const QByteArray& input)
{
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();

    std::string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input.at(i);
		output.push_back('0');
		output.push_back('x');
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
		output.push_back(' ');
    }
    return output;
}

inline QByteArray hex2ba(const std::string& input)
{
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();
    if (len & 1) throw std::invalid_argument("odd length");

    QByteArray output;
    output.reserve(len / 4);
    for (size_t i = 0; i < len; i += 4)
    {
        char a = input[i + 2];
        const char* p = std::lower_bound(lut, lut + 16, a);
        if (*p != a) throw std::invalid_argument("not a hex digit");

        char b = input[i + 1 + 2];
        const char* q = std::lower_bound(lut, lut + 16, b);
        if (*q != b) throw std::invalid_argument("not a hex digit");

        output.push_back(((p - lut) << 4) | (q - lut));
    }
    return output;
}

inline QByteArray val2BCD(int32_t nVal, size_t nDigit)
{
	QByteArray baRet(nDigit/2,0);
	char cByte = 0x00;

	for (int i = nDigit/2 - 1 ; i >=0 ; i--)
	{
		cByte = nVal % 10;
		nVal  = nVal / 10;
		cByte = (nVal % 10)*16 + cByte;
		nVal  = nVal / 10;
		baRet[i] = cByte;
	}
	return baRet;
}

inline int32_t BCD2val(QByteArray baNybbles)
{
	int32_t result(0);
	for (int i = 0; i < baNybbles.size(); ++i) 
	{
		unsigned char nubble = baNybbles.at(i);
		result = result * 100 + (nubble >> 4) * 10 + (nubble & 15);
	}
	return result;
}

inline QByteArray utf8ToDOS(std::string sIn)
{
	QByteArray baOut;
	QString str(sIn.c_str());
	QTextCodec *codec = QTextCodec::codecForName("IBM 866");
	if (codec)
		baOut  = codec->fromUnicode(str);
	else
		baOut = QByteArray(sIn.c_str());
	return baOut;
}





