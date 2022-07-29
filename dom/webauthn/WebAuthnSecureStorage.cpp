#include "WebAuthnSecureStorage.h"
#include <cstdio>
#include <map>
// #include "nsCOMPtr.h"

// nsTHashMap<nsAutoCString, nsAutoCString>* WebAuthnSecureStorage::storage = nullptr;
namespace mozilla {
namespace dom {

WebAuthnSecureStorage::WebAuthnSecureStorage() {
  storage = std::map<nsAutoCString, nsAutoCString>();
  printf("WebAuthnSecureStorage::WebAuthnSecureStorage -- created\n");
}

WebAuthnSecureStorage::~WebAuthnSecureStorage() {
  delete gInstance;
}

WebAuthnSecureStorage* WebAuthnSecureStorage::gInstance;

WebAuthnSecureStorage* WebAuthnSecureStorage::GetInstance() {
  if (!gInstance) {
    gInstance = new WebAuthnSecureStorage();
  }
  return gInstance;
}

nsresult WebAuthnSecureStorage::SetSecureOptions(nsAutoCString host, nsCString options) {
  storage.insert(std::make_pair(host, options));
  printf("WebAuthnSecureStorage::SetSecureOptions -- set %s to %s\n", host.get(), options.get());
  return NS_OK;
}

} }