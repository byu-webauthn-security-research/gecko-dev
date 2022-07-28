#include "WebAuthnSecureStorage.h"
#include <cstdio>
// #include "nsCOMPtr.h"

// nsTHashMap<nsAutoCString, nsAutoCString>* WebAuthnSecureStorage::storage = nullptr;
namespace mozilla {
namespace dom {

WebAuthnSecureStorage::WebAuthnSecureStorage() {
  printf("WebAuthnSecureStorage created\n");
}

WebAuthnSecureStorage::~WebAuthnSecureStorage() {
  delete gSecureStorage;
}

WebAuthnSecureStorage* WebAuthnSecureStorage::gSecureStorage;

WebAuthnSecureStorage* WebAuthnSecureStorage::GetInstance() {
  if (!gSecureStorage) {
    gSecureStorage = new WebAuthnSecureStorage();
  }
  return gSecureStorage;
}

} }