#include <ntqstring.h>
#include <tdewallet.h>
#include <tdeapplication.h>
#include <tdeaboutdata.h>
#include <tdecmdlineargs.h>
#include <iostream>

static const TDECmdLineOptions options[] =
  {
    { "+[arg]", 0, 0 },
     TDECmdLineLastOption // End of options.
  };


int main(int argc, char ** argv)
{

   if (argc < 2) {
      std::cerr << "Wrong number of arguments. Expecting 2 instead of " << argc
                << std::endl;
      return 1;
   }
   if (TQString(argv[1]).contains("Bad passphrase")) {
      std::cerr << "Bad passphrase seen in request. Exiting." << std::endl;
      return 1;
   }
   if (not TQString(argv[1]).contains("Enter passphrase for")) {
      std::cerr << "Could not find standard \"Enter passphrase for\" in request"
                << ". Exiting" << std::endl;
      return 1;
   }

   TDEAboutData aboutData("askpass", I18N_NOOP("TDEDE SSH TDEWallet Askpass"),
                         "", "", TDEAboutData::License_GPL,
                         "");

   TDECmdLineArgs::init( argc, argv, &aboutData );
   TDECmdLineArgs::addCmdLineOptions( options );

   TDEApplication app(false, false);

   TQString localwallet = TDEWallet::Wallet::LocalWallet();

   TDEWallet::Wallet * wallet =  TDEWallet::Wallet::openWallet(localwallet);

   if (not wallet) {
      std::cerr << "Could not open the standard wallet. Exiting." << std::endl;
      delete wallet;
      return 1;
   }
   if (not wallet->hasFolder("SSH Askpass")) {
      std::cerr << "Could not find an \"SSH Askpass\" folder. Exiting."
                << std::endl;
      delete wallet;
      return 1;
   }
   if (not wallet->setFolder("SSH Askpass")) {
      std::cerr << "Could not switch to folder \"SSH Askpass\". Exiting."
                << std::endl;
      delete wallet;
      return 1;
   }

   TQString key = "ssh-key";
   if (not wallet->hasEntry(key)) {
      std::cerr << "Folder \"SSH Askpass\" does not have entry \"ssh-key\"."
                << " Exiting." << std::endl;
      delete wallet;
      return 1;
   }

   TQString password = "";
   int ret = wallet->readPassword(key, password);
   if (ret != 0) {
      std::cerr << "Could not read password for key \"ssh-key\" in folder "
                << "\"SSH Askpass\". Exiting." << std::endl;
      delete wallet;
      return 1;
   }
   std::cout << password.latin1() << std::endl;
   delete wallet;
   return 0;
}
