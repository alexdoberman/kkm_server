#include "kkm_application.h"
//#include "../../kkm_logger/easylogging++.h"

//_INITIALIZE_EASYLOGGINGPP
int main(int argc, char** argv)
{
	//LOG(INFO) << "My first info log using default logger";
	KKMServer app;
	return app.run(argc, argv);
}
