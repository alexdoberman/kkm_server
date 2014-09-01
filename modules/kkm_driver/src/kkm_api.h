#pragma once
#include "kkm_driver.h"
#include "kkm_driver_def.h"

#include <memory>

#include <QtSerialPort/QSerialPort>
#include <QByteArray>

class IKKMApi
{
public:
	virtual ~IKKMApi() {};
	virtual TResult exec_command(const QByteArray& baCmd, QByteArray& baAnswer, int nTimeOut) = 0;
};

typedef std::unique_ptr<IKKMApi> TSmartKKMApi;

class CKKMApi: public IKKMApi
{
public:
	explicit CKKMApi(QSerialPort  *pSerial, TKKMInfo kkmDesc);
	virtual ~CKKMApi();
public:
	virtual TResult exec_command(const QByteArray& baCmd, QByteArray& baAnswer, int nTimeOut = defs::nReadENQTimeout);

private:
	TResult sendENQ_readACK();
	TResult sendACK();
    TResult readACK();
	TResult sendEOT();
	TResult readEOT();
	TResult sendENQ();
	TResult readENQ(int nTimeOut);
	TResult loadKKMData(QByteArray& baOutData);
	TResult readChunk(QByteArray& baOutData, int nTimeOut);
	QByteArray makeCommand(const QByteArray & baData);

private:
	QSerialPort  *m_pSerial;
	TKKMInfo      m_kkmDesc;
};

//TSmartKKMApi createKKMApi(TKKMInfo kkmDesc);

QByteArray maskCommand(const QByteArray& baIn);
QByteArray demaskCommand(const QByteArray& baIn);

