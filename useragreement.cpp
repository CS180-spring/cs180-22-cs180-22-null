#include "useragreement.h"

#include <algorithm>
#include <iostream>
#include <string>

bool getUserAgreement() {
  std::string agreement = R"(User Agreement

By using the NullDB software (referred to as "the software" or "NullDB"), you agree to the following terms and conditions:

1. Data Responsibility: The current version of NullDB is provided as-is, without any warranty or guarantee of data integrity. We are not responsible for any data loss or corruption that may occur while using the software. It is the user's responsibility to ensure the safety and backup of their data.

2. User Account Information: Users are responsible for remembering their login information, including usernames and passwords. We do not store or have access to user passwords. If a password is lost or forgotten, it cannot be recovered, and the user may lose access to their account and associated data.

3. Record ID: Users should take note of the unique Record ID associated with each entry. Losing or forgetting a Record ID may result in difficulty locating or retrieving specific records.

4. Encryption Key: If encryption is implemented in NullDB, users are solely responsible for keeping their encryption key secure. We do not store or have access to encryption keys. Losing or forgetting an encryption key may result in permanent data loss, as we cannot recover encrypted data without the correct encryption key.

5. Importance of Data: Until further notice that the software is deemed stable, users are advised not to store any important or irreplaceable data within NullDB. The current version is under development and may have potential bugs or issues that could lead to data loss. Users should exercise caution and use the software for non-critical purposes.

6. Data Backup: It is recommended that users adopt a habit of regularly backing up their data. We suggest following the 3-2-1 backup strategy, which involves having three copies of your data, stored on at least two different storage media, with one copy stored remotely. This practice helps to mitigate the risk of data loss.

7. User Conduct: Users of NullDB are expected to comply with all applicable laws and regulations while using the software. Any misuse or unauthorized activities are strictly prohibited. We reserve the right to terminate or suspend user accounts in cases of violation or suspected misuse.

8. Limitation of Liability: In no event shall we be liable for any direct, indirect, incidental, consequential, special, or exemplary damages arising out of or in connection with the use or performance of the software, even if we have been advised of the possibility of such damages.

9. Changes to the Agreement: We reserve the right to modify or update this user agreement at any time without prior notice. Users are encouraged to review this agreement periodically for any changes.

By using NullDB, you acknowledge that you have read, understood, and agreed to the terms and conditions outlined in this user agreement. If you do not agree with these terms, you should refrain from using the software.

If you have any questions or concerns regarding this agreement, please contact us.

Last updated: 5/12/2023)";

  std::string userResponse;
  std::cout << std::endl << agreement << std::endl;
  std::cout << "\nDo you agree with these terms? (YES/NO): ";
  std::getline(std::cin, userResponse);

  std::transform(userResponse.begin(), userResponse.end(), userResponse.begin(),
                 ::toupper);
  if (userResponse == "YES" || userResponse == "Y") {
    return true;
  } else {
    return false;
  }
}
