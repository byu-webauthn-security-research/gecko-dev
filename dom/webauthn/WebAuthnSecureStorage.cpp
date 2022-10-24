#include "mozilla/dom/WebAuthnSecureStorage.h"
// #include "mozilla/ipc/BackgroundParent.h"
#include "mozilla/dom/PWebAuthnTransaction.h"
#include "nsStringFwd.h"
#include <cstdio>
#include <iostream>
#include <thread>
#include <map>
#include <utility>
#include <string>
#include <algorithm>
#include <thread>
#include <sstream>
#include <cstdint>

#include "mozilla/ipc/BackgroundParent.h"
#include "hasht.h"
#include "nsHTMLDocument.h"
#include "nsIURIMutator.h"
#include "nsThreadUtils.h"
#include "WebAuthnCoseIdentifiers.h"
#include "mozilla/BasePrincipal.h"
#include "mozilla/dom/AuthenticatorAssertionResponse.h"
#include "mozilla/dom/AuthenticatorAttestationResponse.h"
#include "mozilla/dom/PublicKeyCredential.h"
#include "mozilla/dom/Promise.h"
#include "mozilla/dom/PWebAuthnTransaction.h"
#include "mozilla/dom/WebAuthnManager.h"
#include "mozilla/dom/WebAuthenticationBinding.h"
#include "mozilla/dom/WebAuthnTransactionChild.h"
#include "mozilla/dom/WebAuthnUtil.h"
#include "mozilla/ipc/BackgroundChild.h"
#include "mozilla/ipc/PBackgroundChild.h"
#include "authenticator/src/u2fhid-capi.h"
#include <stdint.h> 
// #include <unistd.h>
// #include "nsCOMPtr.h"

// nsTHashMap<nsAutoCString, nsAutoCString>* WebAuthnSecureStorage::storage = nullptr;
namespace mozilla::dom {

WebAuthnSecureStorage::WebAuthnSecureStorage() {
  // storage = std::map<nsAutoCString, nsAutoCString>();
  printf("WebAuthnSecureStorage::WebAuthnSecureStorage -- created\n");
}

static StaticRefPtr<WebAuthnSecureStorage> gWebAuthnSecureStorage;


WebAuthnSecureStorage* WebAuthnSecureStorage::GetInstance() { // get Instance
  // mozilla::ipc::AssertIsInMainProcess();
  // AssertIsOnMainThread();
  printf("WebAuthnSecureStorage::GetInstance -- thread: ");
  // std::cout << std::this_thread::get_id() << " process: " << getpid() << std::endl;
  printf("WebAuthnSecureStorage::GetInstance -- gInstance: %p\n", gWebAuthnSecureStorage.get());
  if (!gWebAuthnSecureStorage) {
    auto secureStorage = MakeRefPtr<WebAuthnSecureStorage>();
    gWebAuthnSecureStorage = secureStorage.get();
  }
  printf("WebAuthnSecureStorage::GetInstance -- gInstance: %p\n", gWebAuthnSecureStorage.get());
  return gWebAuthnSecureStorage;
}

// int WebAuthnSecureStorage::AddRef() { return 1; }

// int WebAuthnSecureStorage::Release() { return 0; }

nsresult WebAuthnSecureStorage::SetSecureOptions(nsCString options) {
  printf("WebAuthnSecureStorage::SetSecureOptions -- entered function \n");

  this->Options = std::move(options);
  this->responsePairs.clear();
  WebAuthnSecureStorage::SerializeSecureOptions2();
  //MakeCredential();

  //std::cout << "WebAuthnSecureStorage::SetSecureOptions -- After serializing" <<  std::endl;
  return NS_OK;
}
nsCString WebAuthnSecureStorage::StringToNsString(std::string input){
  nsCString output;
  for (unsigned int i=0;i<input.length(); i++ ){
      char currChar =input.at(i);
      output.Insert(currChar, output.Length());
    }
  return output;
}
nsTString<char16_t> WebAuthnSecureStorage::StringToNsTChar(std::string input){
  nsTString<char16_t> output;
  for (unsigned int i=0;i<input.length(); i++ ){
      char currChar =input.at(i);
      output.Insert(currChar, output.Length());
    }
  return output;
}
long WebAuthnSecureStorage::StringToLong(std::string input){
  //std::cout<< "string:" << input<< std::endl;
  long output;
  output = stoul(input);
  //std::cout<< "long:" << output<< std::endl;
  return output;
}
nsAutoCString WebAuthnSecureStorage::toAutoCString(std::string input){
  nsAutoCString output;
  for (unsigned int i=0;i<input.length(); i++ ){
      char currChar =input.at(i);
      output.Insert(currChar, output.Length());
    }
  return output;
}

void WebAuthnSecureStorage::SerializeSecureOptions2(){
  std::string stdString;
    for (unsigned int i=0;i<this->Options.Length(); i++ ){ // create a standard string to be able to substring
      // std::cout<<this->Options.CharAt(i) << " ";
      char currChar =this->Options.CharAt(i);
      stdString.insert(stdString.length(), 1, currChar);
    }
    this->responseStorage = {};
    // success
    std::string currString = stdString;
    std::string leftString = "";
    std::size_t delim = currString.find(',');
    leftString = currString.substr(0, delim);
    currString =currString.substr(delim + 1);
    leftString = GetKeyValuePairs(leftString); // success result;
    if (leftString.compare("true") ==0){
      this->responseStorage.success = true;
    }
    else{
      this->responseStorage.success = false;
    }

    //request
      //username
      delim = currString.find(',');
      leftString = currString.substr(0, delim);
      currString =currString.substr(delim + 1);
      delim = leftString.find(':');
      leftString = leftString.substr(delim+1);
      leftString = GetKeyValuePairs(leftString); // request username result;

      this->responseStorage.request.username = leftString;

      //credentialNickname
      delim = currString.find(',');
      leftString = currString.substr(0, delim);
      currString =currString.substr(delim + 1);
      leftString = GetKeyValuePairs(leftString); // request credNickname result;
      this->responseStorage.request.credentialNickname = leftString;
      //requestId
      delim = currString.find(',');
      leftString = currString.substr(0, delim);
      currString =currString.substr(delim + 1);
      leftString = GetKeyValuePairs(leftString); // request credNickname result;
      this->responseStorage.request.requestId = leftString;
      //publicKeyCredentialCreationOptions
      delim = currString.find(',');
      leftString = currString.substr(0, delim);
      currString =currString.substr(delim + 1);
      delim = leftString.find(':');
      leftString = leftString.substr(delim+1); // request publicKeyCredentialCreationOptions r
        //rp
          delim = leftString.find(':');
          leftString = leftString.substr(delim+1);
          //name
          leftString = GetKeyValuePairs(leftString); // request publicKeyCredentialCreationOptions rp name result;
          this->responseStorage.request.optionsStorage.rpStorage.name = leftString;
          //id
          delim = currString.find(',');
          leftString = currString.substr(0, delim);
          currString =currString.substr(delim + 1);
          leftString = GetKeyValuePairs(leftString);
          this->responseStorage.request.optionsStorage.rpStorage.id = leftString;
        //user
        delim = currString.find(',');
        leftString = currString.substr(0, delim);
        currString =currString.substr(delim + 1);
          //name
          delim = leftString.find(':');
          leftString = leftString.substr(delim+1);
          leftString = GetKeyValuePairs(leftString); // request publicKeyCredentialCreationOptions user name result;
          this->responseStorage.request.optionsStorage.userStorage.name = leftString;
          //displayName
          delim = currString.find(',');
          leftString = currString.substr(0, delim);
          currString =currString.substr(delim + 1);
          leftString = GetKeyValuePairs(leftString);
          this->responseStorage.request.optionsStorage.userStorage.displayName = leftString;
          //id
          delim = currString.find(',');
          leftString = currString.substr(0, delim);
          currString =currString.substr(delim + 1);
          leftString = GetKeyValuePairs(leftString);
          this->responseStorage.request.optionsStorage.userStorage.id = leftString;
        //challenge
        delim = currString.find(',');
        leftString = currString.substr(0, delim);
        currString =currString.substr(delim + 1);
        leftString = GetKeyValuePairs(leftString);
        this->responseStorage.request.optionsStorage.challenge = leftString;
        //pubKeyCredParams
        delim = currString.find(',');
        leftString = currString.substr(0,delim);
        currString =currString.substr(delim + 1);
        int counter =0;
        delim = leftString.find('[');
        leftString = leftString.substr(delim+1);
        leftString = GetKeyValuePairs(leftString);
        this->responseStorage.request.optionsStorage.paramsStorage[0].alg = leftString;
        while (currString.find("alg") != std::string::npos){
          //for each
          //agl
          //type
          delim = currString.find(',');
          leftString = currString.substr(0, delim);
          currString =currString.substr(delim + 1);
          leftString = GetKeyValuePairs(leftString);
          this->responseStorage.request.optionsStorage.paramsStorage[counter].type = leftString;
          delim = currString.find(',');
          leftString = currString.substr(0, delim);
          currString =currString.substr(delim + 1);
          leftString = GetKeyValuePairs(leftString);
          this->responseStorage.request.optionsStorage.paramsStorage[counter].alg = leftString;
          counter++;
        }
        //type
        delim = currString.find(',');
        leftString = currString.substr(0, delim);
        currString =currString.substr(delim + 1);
        leftString = GetKeyValuePairs(leftString);
        //exclude credentials
        delim = currString.find(',');
        leftString = currString.substr(0, delim);
        currString =currString.substr(delim + 1);
        //authenticatorSelection
          delim = currString.find(',');
          leftString = currString.substr(0, delim);
          currString =currString.substr(delim + 1);
          //residentKey
          delim = leftString.find(':');
          leftString = leftString.substr(delim+1);
          leftString = GetKeyValuePairs(leftString);
          this->responseStorage.request.optionsStorage.authentictorSelection.residentKey = leftString;
        //attestation
        delim = currString.find(',');
        leftString = currString.substr(0, delim);
        currString =currString.substr(delim + 1);
        leftString = GetKeyValuePairs(leftString);
        this->responseStorage.request.optionsStorage.attestationStorage = leftString;
        //extensions
        delim = currString.find(',');
        leftString = currString.substr(0, delim);
        currString =currString.substr(delim + 1);
          //credProps
          delim = leftString.find(':');
          leftString = leftString.substr(delim+1);
          leftString = GetKeyValuePairs(leftString);
          if (leftString.compare("true") ==0){
            this->responseStorage.request.optionsStorage.extensions.credProps = true;
          }
          else{
            this->responseStorage.request.optionsStorage.extensions.credProps = false;
          }
      //session Token
      delim = currString.find(',');
      leftString = currString.substr(0, delim);
      currString =currString.substr(delim + 1);
      leftString = GetKeyValuePairs(leftString);
      this->responseStorage.request.sessionToken = leftString;
    //actions
    delim = currString.find(',');
    leftString = currString.substr(0, delim);
    currString =currString.substr(delim + 1);
      //finish
      delim = leftString.find(':');
      leftString = leftString.substr(delim+1);
      leftString = GetKeyValuePairs(leftString);
      this->responseStorage.actions.finish = leftString;
    //PrintStorage();
}
void WebAuthnSecureStorage::PrintStorage(){
  std::cout<< "Success: "<<this->responseStorage.success << "," << std::endl;
  std::cout<< "request:" << std::endl;
  std::cout <<"username:" <<this->responseStorage.request.username << "," << std::endl;
  std::cout <<"credentialNickname:" <<this->responseStorage.request.credentialNickname << "," << std::endl;
  std::cout <<"requestId:" <<this->responseStorage.request.requestId << "," << std::endl;
  std::cout <<"options Storage:" << std::endl;
  std::cout <<"rpstorage:" << std::endl;
  std::cout <<"name:" <<this->responseStorage.request.optionsStorage.rpStorage.name << "," << std::endl;
  std::cout <<"id:" <<this->responseStorage.request.optionsStorage.rpStorage.id << "," << std::endl;
  std::cout <<"user storage:" << std::endl;
  std::cout <<"name:" <<this->responseStorage.request.optionsStorage.userStorage.name << "," << std::endl;
  std::cout <<"dislay name:" <<this->responseStorage.request.optionsStorage.userStorage.displayName << "," << std::endl;
  std::cout <<"id:" <<this->responseStorage.request.optionsStorage.userStorage.displayName<< "," << std::endl;
  std::cout <<"challenge:" <<this->responseStorage.request.optionsStorage.challenge<< "," << std::endl;
  std::cout << "Params array" << std::endl;
  std::cout << "Params 0:" <<this->responseStorage.request.optionsStorage.paramsStorage[0].alg << ", " <<this->responseStorage.request.optionsStorage.paramsStorage[0].type << std::endl;
  std::cout << "Params 1:" <<this->responseStorage.request.optionsStorage.paramsStorage[1].alg << ", " <<this->responseStorage.request.optionsStorage.paramsStorage[1].type << std::endl;
  std::cout << "Params 2:" <<this->responseStorage.request.optionsStorage.paramsStorage[2].alg << ", " <<this->responseStorage.request.optionsStorage.paramsStorage[2].type << std::endl;
  std::cout<< "authenticator selection "<< std::endl;
  std::cout <<"resident key:" <<this->responseStorage.request.optionsStorage.authentictorSelection.residentKey<< "," << std::endl;
  std::cout <<"attestation storage:" <<this->responseStorage.request.optionsStorage.attestationStorage<< "," << std::endl;
  std::cout<< "extensions: "<< std::endl;
  std::cout <<"extensions storage:" <<this->responseStorage.request.optionsStorage.extensions.credProps<< "," << std::endl;
  std::cout <<"session token:" <<this->responseStorage.request.sessionToken<< "," << std::endl;
  std::cout<< "actions"<< std::endl;
  std::cout <<"actions finish:" <<this->responseStorage.actions.finish<< "," << std::endl;
}

void WebAuthnSecureStorage::SerializeSecureOptions(){
    std::string stdString;
    for (unsigned int i=0;i<this->Options.Length(); i++ ){ // create a standard string to be able to substring
      // std::cout<<this->Options.CharAt(i) << " ";
      char currChar =this->Options.CharAt(i);
      stdString.insert(stdString.length(), 1, currChar);
    }
    //std::cout<<"stdString parsing -- secure storage" <<std::endl;
    //std::cout<<this->Options <<std::endl;

    std::string currString = stdString;
    std::string leftString = "";

    //check if success : true before even entering in
    std::cout << currString.substr(11, 4) <<std::endl; //should be true
    currString = currString.substr(27);
    do{
      std::size_t delim = currString.find(',');
      leftString = currString.substr(0, delim);
      currString =currString.substr(delim + 1);
      //std::cout<<leftString <<std::endl;
      if (delim == std::string::npos){
        //std::cout<<"currString : "<< currString <<std::endl;
        //GetKeyValuePairs(currString);
        continue;
      }
      if (leftString.find("pubKeyCredParams") != std::string::npos){
        //std::cout<< "pubkeyCred is found" << std::endl; // end case
        break;
      }
      GetKeyValuePairs(leftString);

    }
    while(currString != "");

}
std::string WebAuthnSecureStorage::GetKeyValuePairs(std::string keyValue){
  std::size_t valueIndex;
  valueIndex =keyValue.find(':');
  std::string key =keyValue.substr(0, valueIndex);
  std::string value =keyValue.substr(valueIndex+1, keyValue.length());
  std::string keyFixed;
  std::string valueFixed;
  for (unsigned int i=0;i<key.length(); i++ ){
    if (std::isalnum(key[i])){
      keyFixed.insert(keyFixed.length(), 1, key[i]);
    }
  }
  // // fix value using alnum
  //if (keyFixed == "requestId" || keyFixed =="id" || keyFixed == "challenge"){
    //printf("requestId found\n");
    for (unsigned int i=0;i<value.length(); i++ ){
      if (value[i] == '\"'){
        for (unsigned int j=i + 1;j<value.length(); j++ ){
          if (value[j] != '\"' && value[j] != '}'){
            valueFixed.insert(valueFixed.length(), 1, value[j]);
          }
          else{
            break;
          }
        }
      }
    }
  if (keyFixed == "success" || keyFixed == "alg" ){
    valueFixed = value;
    //std::cout<<"string:  "<< value <<std::endl;
  }
  if (keyFixed == "credProps" ){
    //std::cout<<"string:  "<< value <<std::endl;
    for (unsigned int i=0;i<value.length(); i++ ){
      if (std::isalnum(value[i])){
        valueFixed.insert(valueFixed.length(), 1,value[i]);
      }
    }
  }
  return valueFixed;
}
std::string WebAuthnSecureStorage::Advance(std::string value){
  //std::cout<<"string:  "<< value <<std::endl;
  std::size_t valueIndex;
  valueIndex =value.find(':');
  return value.substr(valueIndex+1);

}

void WebAuthnSecureStorage::MapValue(){
  printf("WebAuthnSecureStorage::Map Values -- Map iteration\n");
    for (auto const& x : this->responsePairs)
    {
        std::cout << x.first  // string (key)
                  << ':'
                  << x.second // string's value
                  << std::endl;
        //if (strcmp(x.first,"requestId" )==0){
          //this->RpId = x.second;
        //}
    }
}

nsCString WebAuthnSecureStorage::GetSecureOptions() {
  printf("WebAuthnSecureStorage::GetSecureOptions -- entered function\n");

  // nsAutoCString host_t = nsAutoCString(host.get());
  // printf("WebAuthnSecureStorage::GetSecureOptions -- map: %s\n", storage.cbegin()->first.get());
  // printf("WebAuthnSecureStorage::GetSecureOptions -- host: %s\n", host_t.get());
  // options = &(storage.at(host_t));
  // printf("WebAuthnSecureStorage::GetSecureOptions -- retrieved %s as %s\n", host_t.get(), options->get());
  //std::cout << "WebAuthnSecureStorage::GetSecureOptions -- Options: "<< this->Options <<  std::endl;
  //WebAuthnSecureStorage::MapValue();
  return Options;
}
// void WebAuthnSecureStorage::CreateInfo(nsString origin, nsAutoCString clientDataJSON,
//       uint32_t adjustedTimeout,nsTArray<WebAuthnScopedCredential> excludeList, WebAuthnMakeCredentialExtraInfo extra, BrowsingContext * context){
//   WebAuthnMakeCredentialInfo info(origin, NS_ConvertUTF8toUTF16(this->RpId), this->responsePairs.at("challenge"), clientDataJSON, adjustedTimeout, excludeList, Some(extra), context->Top()->Id());
//   this->Info = info;
// }

uint64_t WebAuthnSecureStorage::GetID(){
  return this->RpIDInt;
}
void WebAuthnSecureStorage::SetInfo(WebAuthnMakeCredentialInfo info){ // WebAuthnMakeCredentialInfo
  printf("WebAuthnSecureStorage::SetInfo -- entered function\n");
  this->Info = info;
  std::cout << "Info settings : " << info.RpId() <<std::endl;
}
WebAuthnMakeCredentialInfo WebAuthnSecureStorage::GetInfo(){
  // return info
  return this->Info;
}

void WebAuthnSecureStorage::MakeCredential(const char * ClientData){
  std::cout<< "Client Data: "<< ClientData << std::endl;
  nsString origin;
  // STEP 2.5 rpID
  nsCString rpId;
  
  //nsresult rv = GetOrigin(mParent, origin, rpId);
  rpId = StringToNsString(this->responseStorage.request.optionsStorage.rpStorage.id);
  std::cout << "my rp id : "<< rpId << " versus the header: " <<std::endl;
  printf("WebAuthnSecureStorage::Make Credential -- entered function\n");


  CryptoBuffer userId;
  userId.Assign(StringToNsString(this->responseStorage.request.optionsStorage.userStorage.id)); // change to mId from key value user:{name, displayname, ID}
  if (userId.Length() > 64) {
    printf("user.id is too long\n");
    return;
  }


  uint32_t adjustedTimeout = 30000;

  nsTArray<CoseAlg> coseAlgos; // need to finish serializing
  if (this->responseStorage.request.optionsStorage.paramsStorage[0].type == "") { // pull instead from key value pairs
    coseAlgos.AppendElement(static_cast<long>(CoseAlgorithmIdentifier::ES256));
    coseAlgos.AppendElement(static_cast<long>(CoseAlgorithmIdentifier::RS256));
  } else {
    for (size_t a = 0; a < 3; ++a) {
      if (this->responseStorage.request.optionsStorage.paramsStorage[a].type != "public-key") {
        //std::cout << "not appended : " << this->responseStorage.request.optionsStorage.paramsStorage[a].type << std::endl;
        continue;
      }
      //std::cout << " appended : " << this->responseStorage.request.optionsStorage.paramsStorage[a].type << std::endl;
      if (this->responseStorage.request.optionsStorage.paramsStorage[a].type != ""){
        //printf("actually appended\n");
        coseAlgos.AppendElement(StringToLong(this->responseStorage.request.optionsStorage.paramsStorage[a].alg));
      }
    }
  }

  if (coseAlgos.IsEmpty() && this->responseStorage.request.optionsStorage.paramsStorage[0].type != "") {
    printf("break 3\n");
    return;
  }

  CryptoBuffer challenge;
  nsAutoCString clientDataJSON;
  AuthenticationExtensionsClientInputs mExtensions;
  if (!challenge.Assign(StringToNsString(this->responseStorage.request.optionsStorage.challenge))) {
    printf("break 4\n");
    
    return;
  }
  std::string data(ClientData);
  clientDataJSON = toAutoCString(data);
  std::cout << "client data: " << clientDataJSON << std::endl;
  std::string stdString;
  for (unsigned int i=0;i<clientDataJSON.Length(); i++ ){ // create a standard string to be able to substring
    // std::cout<<this->Options.CharAt(i) << " ";
    char currChar =clientDataJSON.CharAt(i);
    stdString.insert(stdString.length(), 1, currChar);
  }
  std::string leftString;
  std::size_t delim = stdString.find(',');
  leftString = stdString.substr(0, delim);
  leftString = GetKeyValuePairs(leftString);
  origin = StringToNsTChar(leftString);
  // if (NS_WARN_IF(NS_FAILED(srv))) {
  //   printf("break 4\n");
  //   return;
  // }

  // STEP 2.5 excludeList
  nsTArray<WebAuthnScopedCredential> excludeList;
  // for (const auto& s : aOptions.mExcludeCredentials) { // dont do yet
  //   WebAuthnScopedCredential c;
  //   CryptoBuffer cb;
  //   cb.Assign(s.mId);
  //   c.id() = cb;
  //   excludeList.AppendElement(c);
  // }

  // if (!MaybeCreateBackgroundActor()) {
  //   printf("break 5\n");
  //   return;
  // }

  // TODO: Add extension list building
  nsTArray<WebAuthnExtension> extensions;
  AuthenticatorSelectionCriteria selection;
  //AuthenticatorAttachment::Platform);
  selection.mRequireResidentKey = true;
  // find user verification
  if (this->responseStorage.request.optionsStorage.authentictorSelection.residentKey.compare("required") ==0){
    std::cout << "required key " << std::endl;
    selection.mUserVerification = UserVerificationRequirement::Required;
  }
  if (this->responseStorage.request.optionsStorage.authentictorSelection.residentKey.compare("preferred") ==0){
    std::cout << "preferred key " << std::endl;
    selection.mUserVerification = UserVerificationRequirement::Preferred;
  }
  if (this->responseStorage.request.optionsStorage.authentictorSelection.residentKey.compare("discouraged") ==0){
    std::cout << "discouraged key " << std::endl;
    selection.mUserVerification = UserVerificationRequirement::Discouraged;
  }
  if (this->responseStorage.request.optionsStorage.authentictorSelection.residentKey.compare("endguard") ==0){
    std::cout << "endguard key " << std::endl;
    selection.mUserVerification = UserVerificationRequirement::EndGuard_;
  }
  const auto& attachment = selection.mAuthenticatorAttachment;
  // Attachment
  Maybe<AuthenticatorAttachment> authenticatorAttachment;
  if (attachment.WasPassed()) {
    authenticatorAttachment.emplace(attachment.Value());
  }

  // Create and forward authenticator selection criteria. // NEED TO OVERWRITE
  WebAuthnAuthenticatorSelection authSelection(selection.mRequireResidentKey,
                                               selection.mUserVerification,
                                               authenticatorAttachment);


  nsString rpIcon;
  // // if (true) {
  // //   rpIcon = aOptions.mRp.mIcon.Value();
  // // }

   nsString userIcon;
  // // if (aOptions.mUser.mIcon.WasPassed()) {
  // //   userIcon = aOptions.mUser.mIcon.Value();
  // // }
  AttestationConveyancePreference attestation;
  if (this->responseStorage.request.optionsStorage.attestationStorage.compare("none") ==0){
    attestation = AttestationConveyancePreference::None;
  }
  if (this->responseStorage.request.optionsStorage.attestationStorage.compare("indirect") ==0){
    attestation = AttestationConveyancePreference::Indirect;
  }
  if (this->responseStorage.request.optionsStorage.attestationStorage.compare("direct") ==0){
    attestation = AttestationConveyancePreference::Direct;
  }
  if (this->responseStorage.request.optionsStorage.attestationStorage.compare("endguard") ==0 ||this->responseStorage.request.optionsStorage.attestationStorage.compare("endguard_") ==0 ){
    attestation = AttestationConveyancePreference::EndGuard_;
  }
  WebAuthnMakeCredentialRpInfo rpInfo(StringToNsTChar(this->responseStorage.request.optionsStorage.rpStorage.name), rpIcon);

  // WebAuthnMakeCredentialUserInfo userInfo(
  //     userId, StringToNsTChar("dan"), userIcon, StringToNsTChar(this->responseStorage.request.optionsStorage.userStorage.displayName));
  // std::cout << " IN SECURE STORAGE ::dan vs :" << this->responseStorage.request.optionsStorage.userStorage.name << std::endl;
  WebAuthnMakeCredentialUserInfo userInfo(
      userId, StringToNsTChar(this->responseStorage.request.optionsStorage.userStorage.name), userIcon, StringToNsTChar(this->responseStorage.request.optionsStorage.userStorage.displayName));

  WebAuthnMakeCredentialExtraInfo extra(rpInfo, userInfo, coseAlgos, extensions,
                                        authSelection, attestation);
  //BrowsingContext* context = mParent->GetBrowsingContext();
  //STEP 2.5 END
  WebAuthnMakeCredentialInfo info( 
      origin, NS_ConvertUTF8toUTF16(rpId), challenge, clientDataJSON,
      adjustedTimeout, excludeList, Some(extra), 1);

  // get assertion

  this->Info = info;
  printf("end of make credential function\n");
}

 void WebAuthnSecureStorage::SetResult(WebAuthnMakeCredentialResult result){
  printf("Setting result\n");
  this->Result = result;
 }

 nsresult WebAuthnSecureStorage::AssembleClientDataStored( const CryptoBuffer& aChallenge,
    const nsAString& aType,
    const AuthenticationExtensionsClientInputs& aExtensions,
    /* out */ nsACString& aJsonOut) {

  nsString challengeBase64;
  nsresult rv = aChallenge.ToJwkBase64(challengeBase64);
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return NS_ERROR_FAILURE;
  }

  CollectedClientData clientDataObject;
  clientDataObject.mType.Assign(aType);
  clientDataObject.mChallenge.Assign(challengeBase64);

  nsAutoString temp;

  aJsonOut.Assign(NS_ConvertUTF16toUTF8(temp));
  return NS_OK;
}




}