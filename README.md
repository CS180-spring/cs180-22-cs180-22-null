# Project-CS180-NULL

<code><img height="20" src="https://shields.io/badge/NullDB-%E2%9C%A8-239DFF?style=flat-square" alt="NullDB"></code>
<code><img height="20" src="https://shields.io/badge/style-Powered%20by%20C%2B%2B-green?logo=C%2B%2B&style=flat-square&label=" alt="Powered by C++" title="Powered by C++"></code>
<code><img height="20" src="https://shields.io/badge/Library-STL-ff69b4?style=flat-square" alt="STL"></code>
<code><img height="20" src="https://shields.io/badge/Library-Algorithm-00cc99?style=flat-square" alt="Algorithm"></code>
<code><img height="20" src="https://shields.io/badge/Library-fstream-ffcc00?style=flat-square" alt="fstream"></code>
<code><img height="20" src="https://shields.io/badge/Library-iostream-ff6600?style=flat-square" alt="iostream"></code>
<code><img height="20" src="https://shields.io/badge/Library-string-cc66ff?style=flat-square" alt="string"></code>
<code><img height="20" src="https://shields.io/badge/Library-vector-9933cc?style=flat-square" alt="vector"></code>

Please read and agree to our user agreement at the end of this document BEFORE using the software.

[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-718a45dd9cf7e7f842a935f5ebbe5719a5e09af4491e668f4dbf3b35d5cca122.svg)](https://classroom.github.com/online_ide?assignment_repo_id=10849768&assignment_repo_type=AssignmentRepo)

Team Members (A-Z):
- Chun Ho Wong
- Cole Shirakata
- Henry Pham
- Irene Wu
- Kevin Lin

## Interface Documentation

### Overview
NullDB is a powerful database solution designed to handle basic text documents with ease. Our innovative platform provides a simple and intuitive way to store and manage text-based data, offering unparalleled performance and reliability.

### Technology
- C++
- Standard Template Library (STL)
- Algorithm
- fstream
- iostream
- string
- vector

### Interface: Terminal

### Features
A text-based database program built with C++. Features include:

- Managing multiple databases
- Basic insert, delete, and update functionality.
- Automatic ID generation and timestamping of records.
- User authentication with username and password, and user-specific record access.
- Encryption for user information and their data.
- Root manager account for creating new user accounts.
- Basic filtering and keyword search for records.
- Data persistence through storing records in a text file.
- Exporting tables and records to JSON files.
- User ID generation for unique identification.
- Inviting users to collaborate on tables.
- Enhanced table details display.
- Integrity checking of records: Detect and mark tampered records.

### Getting Started
Compile and run presentation_layer.cpp. Use the menu to perform actions on records.

### Authentication
Log in with the NullDB system using your unique username and password. A Root manager is granted the ability to create new users and assign specific permissions. Users have access only to the records they created, ensuring data security and privacy.

### Filtering and Searching
NullDB offers robust filtering capabilities. Records can be filtered by their creator, and keywords within the record content can also be searched. This functionality improves data accessibility and user navigation through the system.

### Data Persistence
Our software ensures data persistence by storing records and tables in JSON files. This way, your data remains secure and available even across system restarts or shutdowns.

### Multiple Tables Management
NullDB supports handling multiple tables, allowing users to efficiently manage and categorize their data according to their specific needs. This feature enables broader applications of the software, catering to various data organization requirements.

### Exporting Files
Users can not only export individual records but also entire tables to JSON files. This flexibility enhances data mobility, allowing users to use and share their data externally while maintaining the structural integrity of the records.

### User ID Generation
Upon account creation, each user is assigned a unique ID. This ID facilitates user identification within the system and helps maintain secure access control to the records created by them.

### Collaborating on Tables
To facilitate collaborative work, NullDB users can invite other users to work on the same table by adding them as authorized collaborators. This feature boosts teamwork efficiency while keeping data access controlled and secure.

### Enhanced Table Details Display
We have improved our table details display. It now aligns columns properly for better readability and also shows the owner of the table, adding another layer of information to support efficient data management.

### Integrity Checking
To maintain data reliability, the integrity of records is checked during retrieval. If a record's state is flagged as '2', it is marked as potentially tampered with. Users can still access such records but should consider the flagged status while interpreting the data.

### Deleting Tables
The option to delete tables is available, which also results in the deletion of all the records within it. This feature is useful for managing data storage and decluttering the system of unwanted data.

### Encryption Options
In our latest update, we have increased our focus on data security and have included the addition of multiple encryption options. Encrypted records are treated distinctly during various operations:

- **Encryption Types**: We provide different encryption options for users. Selecting the type of encryption that suits your requirements is now possible.
  
- **Query and Sorting**: During any queries or sorting operations, encrypted records are skipped. This ensures that the confidentiality of the data remains intact even while performing these operations.

- **Viewing Records**: Encrypted records will only display '[Encrypted Data]' when viewing. To view an encrypted record's actual data, you need to view the record individually and provide the correct decryption key.

- **Decryption Key Management**: Upon inserting an encrypted record, users are responsible for remembering their decryption key. This key is essential for viewing the actual data in an encrypted record. Please note that we do not store decryption keys, adding an additional layer of data security.

- **Deletion of Records**: When a user performs a delete operation, the output will display two numbers: the number of records deleted and the number of encrypted records deleted. This ensures that users are informed about the impact of their actions, especially concerning encrypted data.


### Future Enhancements
We are continuously working to make NullDB better. Our future plans include the development of a web frontend for easier user access and interaction with the system.


### BurnDown Chart
[BurnDown Chart](https://docs.google.com/spreadsheets/d/1hRzuSrZP6Vtv3Y083Tg9bsVdBTbVcGTs/edit#gid=2086879608)

### Contributors
All group members contributed equally during the development. We worked on a remote online compiler, which is why the contributor list is off. We have comments to parts where we were in charge. Further code modifications will be done with Github branches and officially tracked.

---

User Agreement

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

Last updated: 06/02/2023
