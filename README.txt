An explanation of the Firefox Source Code Directory Structure and links to
project pages with documentation can be found at:

    https://firefox-source-docs.mozilla.org/contributing/directory_structure.html

For information on how to build Firefox from the source code and create the patch see:

    https://firefox-source-docs.mozilla.org/contributing/contribution_quickref.html

If you have a question about developing Firefox, and can't find the solution
on https://firefox-source-docs.mozilla.org/, you can try asking your question on Matrix at chat.mozilla.org in `Introduction` (https://chat.mozilla.org/#/room/#introduction:mozilla.org) channel.


Nightly development builds can be downloaded from:

    https://archive.mozilla.org/pub/firefox/nightly/latest-mozilla-central/
            - or -
    https://www.mozilla.org/firefox/channel/desktop/#nightly

Keep in mind that nightly builds, which are used by Firefox developers for
testing, may be buggy.


<----------------------- Daniel's Documentation ----------------------->
Step 1. In the nsHttpChannel.cpp function ProcessResponse(),
    take the header webauthn_req_initial and store it inside the singleton storage
Step 2. To mimic the structure of the key from the WebAuthnManager function MakeCredential(),
    I have copied the basic strucutre of the json into a set of structs in the singleton page and then
    made the necessary objects with the Make function in the secure storage singleton.
Step 3. This WebAuthnMakeCredentialInfo object is then injected into the webAuthnTransactionParent function
    RecvRequestRegister() to use the singleton's storage instead of the old info from the original key.
Step 4. This is then received in the WinWebAuthnManager function Register() in the form of a HResult, this is then
    stored as a WebAuthnMakeCredentialResult object in the singleton page.
Step 5. Within the nsHttpChannel, the same function returns the result after ________. bOom dOnE
