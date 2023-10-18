#include "Initialization_Finalization.h"

CK_SESSION_HANDLE InitSession(long slotNumber, char* password)
{
	// Initialize the library
	CK_RV rv = hsm->C_Initialize(nullptr);
	if (rv != CKR_OK) {
		std::cout << "Error initializing the library" << std::endl;
		exit(1);
	}

	// You need to open a session first before generating the key pair
	CK_SESSION_HANDLE session;
	rv = hsm->C_OpenSession(slotNumber, CKF_SERIAL_SESSION | CKF_RW_SESSION, nullptr, nullptr, &session);
	if (rv != CKR_OK) {
		std::cout << "Error opening a session" << std::endl;
		hsm->C_Finalize(nullptr); // Clean up and finalize the library
		exit(1);
	}

	rv = hsm->C_Login(session, CKU_USER, (CK_UTF8CHAR*)password, 9);
	if (rv != CKR_OK) {
		std::cout << "Error logging in to the token" << std::endl;
		hsm->C_CloseSession(session); // Close the session
		hsm->C_Finalize(nullptr); // Clean up and finalize the library
		exit(1);
	}
	return session;
}

void CloseSession(CK_SESSION_HANDLE session)
{
	// Close the session and finalize the library when done
	hsm->C_Logout(session);
	hsm->C_CloseSession(session);
	hsm->C_Finalize(nullptr);
}


