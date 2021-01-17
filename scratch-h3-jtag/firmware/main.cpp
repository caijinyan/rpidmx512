#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "debug.h"

#include "hardware.h"
#include "networkh3emac.h"
#include "ledblink.h"

#include "display.h"

#include "spiflashinstall.h"
#include "spiflashstore.h"
#include "remoteconfig.h"
#include "remoteconfigparams.h"

#include "storenetwork.h"
#include "storeremoteconfig.h"

#include "networkhandleroled.h"

#include "firmwareversion.h"

#include "jamstapl.h"
#include "jamstaplutil.h"

static const char SOFTWARE_VERSION[] = "0.0";

extern "C" {

void notmain(void) {
	Hardware hw;
	NetworkH3emac nw;
	LedBlink lb;
	Display display;
	FirmwareVersion fw(SOFTWARE_VERSION, __DATE__, __TIME__);

	SpiFlashInstall spiFlashInstall;
	SpiFlashStore spiFlashStore;

	fw.Print();
	display.PrintInfo();

	NetworkHandlerOled networkHandlerOled;

	nw.SetNetworkDisplay(&networkHandlerOled);
	nw.SetNetworkStore(StoreNetwork::Get());
	nw.Init(StoreNetwork::Get());
	nw.Print();

	networkHandlerOled.ShowIp();

	RemoteConfig remoteConfig(remoteconfig::Node::RDMNET_LLRP_ONLY, remoteconfig::Output::CONFIG, 0);

	StoreRemoteConfig storeRemoteConfig;
	RemoteConfigParams remoteConfigParams(&storeRemoteConfig);

	if (remoteConfigParams.Load()) {
		remoteConfigParams.Set(&remoteConfig);
		remoteConfigParams.Dump();
	}

	while (spiFlashStore.Flash())
		;

	static constexpr char IDCODE[] = "idcode.jbc";

	auto nFileSize = JamSTAPLUtil::GetFileSize(IDCODE);
	if (nFileSize != 0) {
		auto *pBuffer = new unsigned char[nFileSize];
		if (JamSTAPLUtil::LoadFile(IDCODE, reinterpret_cast<char*>(pBuffer), nFileSize) >= 0) {
			JamSTAPL jbc(pBuffer, nFileSize);
			jbc.CheckCRC(true);
			jbc.PrintInfo();
			jbc.ReadIdCode();
			printf("%s\n%s\n", jbc.GetResultString(), jbc.GetExitCodeString());
		}
		delete[] pBuffer;
	}

	for (;;) {
		nw.Run();
		remoteConfig.Run();
		spiFlashStore.Flash();
		lb.Run();
	}
}

}
