#include "reply_writer.h"

boost::property_tree::ptree kkm_reply_writer::write_reply(const TErrCode& answer)
{
	boost::property_tree::ptree pt;
	pt.put("body.err", answer.nErr);
	pt.put("body.err_kkm", answer.nKKMErr);

	return pt;
}

boost::property_tree::ptree  kkm_reply_writer::write_reply(const TKKMModeReply& answer)
{
	boost::property_tree::ptree pt;
	pt.put("body.mode", answer.nMode);
	pt.put("body.submode", answer.nSubMode);
	pt.put("body.flag", answer.nFlag);
	pt.put("body.err", answer.nErr);
	pt.put("body.err_kkm", answer.nKKMErr);

	return pt;
}

boost::property_tree::ptree kkm_reply_writer::write_reply(const TDevDesc& vPortInfo)
{
	boost::property_tree::ptree pt;
	boost::property_tree::ptree pt_children;

	for (size_t i = 0; i < vPortInfo.size(); i++)
	{
		boost::property_tree::ptree pt_item;
			pt_item.put("port", vPortInfo[i].sPort);
			pt_item.put("location", vPortInfo[i].sLocation);
			pt_item.put("desc", vPortInfo[i].sDescription);
			pt_item.put("manufacturer", vPortInfo[i].sManufacturer);
			pt_item.put("vendorId", vPortInfo[i].sVendorId);
			pt_item.put("productId", vPortInfo[i].sProductId);
			pt_item.put("busy", vPortInfo[i].bBusy);
		
			pt_children.push_back(std::make_pair("", pt_item));
	}
	pt.add_child("body",pt_children);
	return pt;
}

boost::property_tree::ptree  kkm_reply_writer::write_reply(const TExecCommandReply& answer)
{
	boost::property_tree::ptree pt;
	pt.put("body.exec_ansver", answer.sReply);
	pt.put("body.err", answer.nErr);

	return pt;
}

boost::property_tree::ptree  kkm_reply_writer::write_reply(const TKKMStateReply & answer)
{
	boost::property_tree::ptree pt;
	pt.put("body.serial_number", answer.sSerialNumber);
	pt.put("body.kkm_version", answer.sKKMVersion);
	pt.put("body.model", answer.cModel);
	pt.put("body.err", answer.nErr);
	pt.put("body.err_kkm", answer.nKKMErr);

	return pt;
}


