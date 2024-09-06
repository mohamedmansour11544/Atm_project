#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cctype>
#include <vector>
#include <iomanip>
#include <thread>
#include <chrono>
using namespace std;
const string ClientsFileName = "clients.txt";
void MainAtm();
enum UserChoice
{
  qwickwithdraw = 1,
  normalwithdraw = 2,
  depoist = 3,
  checkbalance = 4,
  logout = 5
};
struct sClient
{
  string AccountNumber;
  string PinCode;
  string Name;
  string Phone;
  double AccountBalance;
};
sClient currentclient;
vector<string> splitstring(const string &line, const string &delima)
{
  vector<string> result;
  short pos = 0;
  string word;
  string str = line;
  while ((pos = str.find(delima)) != string::npos)
  {
    word = str.substr(0, pos);
    if (!word.empty())
    {
      result.push_back(word);
    }
    str.erase(0, pos + delima.length());
  }
  if (!str.empty())
  {
    result.push_back(str);
  }
  return result;
}
sClient ConvetLineToRecord(const string &line, const string &delima = "#//#")
{
  sClient client;
  vector<string> result = splitstring(line, delima);
  client.AccountNumber = result[0];
  client.PinCode = result[1];
  client.Name = result[2];
  client.Phone = result[3];
  client.AccountBalance = stod(result[4]);
  return client;
}
string ConvertRecordToLine(const sClient &clientdata, const string &delimiter = "#//#")
{
  return clientdata.AccountNumber + delimiter +
         clientdata.PinCode + delimiter +
         clientdata.Name + delimiter +
         clientdata.Phone + delimiter +
         to_string(clientdata.AccountBalance);
}
vector<sClient> LoadDataFromFile(string ClientsFileName)
{
  fstream myfile;
  string line;
  vector<sClient> VallData;
  myfile.open(ClientsFileName, ios::in);
  if (myfile.is_open())
  {
    while (getline(myfile, line))
    {
      VallData.push_back(ConvetLineToRecord(line));
    }
    myfile.close();
  }
  else
  {
    cout << "Cant open This File " << ClientsFileName << " \n";
  }
  return VallData;
}
void GoToMainMenu()
{
  cout << "\npress any Key To Go Back To Main Menu ...";
  system("pause>0");
  cout << "\n";
  return MainAtm();
}
void SaveClientDataByAccountNumber(string account_number, string FileName, vector<sClient> vClients, double newbalance)
{
  fstream MyFile;
  MyFile.open(FileName, ios::out);
  string DataLine;
  if (MyFile.is_open())
  {
    for (sClient C : vClients)
    {
      if (C.AccountNumber == account_number)
      {
        C.AccountBalance = newbalance; // Refresh New Balance.
      }
      DataLine = ConvertRecordToLine(C);
      MyFile << DataLine << endl;
    }
    MyFile.close();
  }
}

UserChoice ReadChooseMenusScreen(string message)
{
  int choice = 0;
  do
  {
    cout << message << " ";
    cin >> choice;
    cin.ignore();
  } while (choice <= 0 || choice > 5);
  return (UserChoice)choice;
}
void AtmMenuScreen()
{
  system("cls");
  cout << "==========================================\n";
  cout << setw(13) << " " << "Atm Menu Screen\n";
  cout << "==========================================\n";
}
void ShowMenuScreen()
{
  cout << setw(7) << " " << "[1] Quick WithDaraw.\n";
  cout << setw(7) << " " << "[2] Normal WithDaraw.\n";
  cout << setw(7) << " " << "[3] Depoist.\n";
  cout << setw(7) << " " << "[4] Cheak Balance.\n";
  cout << setw(7) << " " << "[5] LogOut.\n";
  cout << "==========================================\n";
}
void sytemloginscreen()
{
  system("cls");
  cout << "---------------------------------------\n";
  cout << setw(10) << " " << "Login Screen\n";
  cout << "---------------------------------------\n";
}
bool FindUserByUsernameAndPassword(string accountnumber, string Password, sClient &client)
{
  vector<sClient> vclient = LoadDataFromFile(ClientsFileName);
  for (sClient c : vclient)
  {
    if (c.AccountNumber == accountnumber && c.PinCode == Password)
    {
      client = c;
      return true;
    }
  }
  return false;
}
void LoadClientData(string accountnumber)
{
  vector<sClient> data = LoadDataFromFile(ClientsFileName);
  for (sClient client : data)
  {
    if (accountnumber == client.AccountNumber)
    {
      currentclient = client; // updating current_active client competed ♥
    }
  }
}
string ReadAccountNumber()
{
  string accountnumber;
  cout << "Enter Account Number? ";
  getline(cin, accountnumber);
  return accountnumber;
}
string ReadPassword()
{
  string password;
  cout << "Enter password? ";
  getline(cin, password);
  return password;
}
void CheakAccount()
{
  sClient cuurnt_client;
  string accountnumber;
  string password;
  bool flagelogin = false;
  do
  {
    sytemloginscreen();
    if (flagelogin)
    {
      cout << "Invalid Account Number/pincode!" << endl;
    }
    accountnumber = ReadAccountNumber();
    password = ReadPassword();
    flagelogin = true;
  } while (!FindUserByUsernameAndPassword(accountnumber, password, cuurnt_client));
  currentclient = cuurnt_client;
}
void CheakBalance()
{
  cout << "================================================\n";
  cout << setw(10) << " " << "Cheak Balance Screen\n";
  cout << "================================================\n";
  cout << "Your Balance is " << currentclient.AccountBalance << " \n";
}
int QuickWithDrawMenu()
{
  int choice;
  cout << "================================================\n";
  cout << setw(10) << " " << "Quick WithDraw Screen\n";
  cout << "================================================\n";
  cout << setw(12) << " " << "[1] 20" << setw(12) << " " << " [2] 50" << endl;
  cout << setw(12) << " " << "[3] 100" << setw(12) << " " << "[4] 200" << endl;
  cout << setw(12) << " " << "[5] 400" << setw(12) << " " << "[6] 600" << endl;
  cout << setw(12) << " " << "[7] 800" << setw(12) << " " << "[8] 1000" << endl;
  cout << setw(12) << " " << "[9] Exit" << endl;
  cout << "================================================\n";
  cout << "Your Balance is " << currentclient.AccountBalance << endl;
  cout << "Choose What With Draw From [1 To 8]? ";
  cin >> choice;
  cin.ignore();
  return choice;
}
bool Allowance_amount(int withdraw_amount, int cuurent_amount)
{
  return cuurent_amount >= withdraw_amount;
}
void QuickWithDraw()
{
  int choice;
  vector<sClient> data = LoadDataFromFile(ClientsFileName);
  char do_operation = 'Y';
  choice = QuickWithDrawMenu();
  int cuurent_amount = currentclient.AccountBalance;
  switch (choice)
  {
  case 1:
    if (Allowance_amount(20, cuurent_amount))
    {
      cout << "\n\n";
      cout << "Are You Sure To Do This Operation? ";
      cin >> do_operation;
      if (toupper(do_operation) == 'Y')
      {
        cuurent_amount -= 20;
        cout << "Done Successfuly ,New Balance is " << cuurent_amount << endl;
      }
      else
      {
        cout << "No Changing In Balance" << endl;
      }
    }
    else
    {
      cout << "Cant Do This Operation Your Balance is " << cuurent_amount << endl;
    }
    break;
  case 2:
    if (Allowance_amount(50, cuurent_amount))
    {
      cout << "\n\n";
      cout << "Are You Sure To Do This Operation? ";
      cin >> do_operation;
      if (toupper(do_operation) == 'Y')
      {
        cuurent_amount -= 50;
        cout << "Done Successfuly ,New Balance is " << cuurent_amount << endl;
      }
      else
      {
        cout << "No Changing In Balance" << endl;
      }
    }
    else
    {
      cout << "Cant Do This Operation Balance is " << cuurent_amount << endl;
    }
    break;
  case 3:
    if (Allowance_amount(100, cuurent_amount))
    {
      cout << "\n\n";
      cout << "Are You Sure To Do This Operation? ";
      cin >> do_operation;
      if (toupper(do_operation) == 'Y')
      {
        cuurent_amount -= 100;
        cout << "Done Successfuly ,New Balance is " << cuurent_amount << endl;
      }
      else
      {
        cout << "No Changing In Balance" << endl;
      }
    }
    else
    {
      cout << "Cant Do This Operation Balance is " << cuurent_amount << endl;
    }
    break;
  case 4:
    if (Allowance_amount(200, cuurent_amount))
    {
      cout << "\n\n";
      cout << "Are You Sure To Do This Operation? ";
      cin >> do_operation;
      if (toupper(do_operation) == 'Y')
      {
        cuurent_amount -= 200;
        cout << "Done Successfuly ,New Balance is " << cuurent_amount << endl;
      }
      else
      {
        cout << "No Changing In Balance" << endl;
      }
    }
    else
    {
      cout << "Cant Do This Operation Balance is " << cuurent_amount << endl;
    }
    break;
  case 5:
    if (Allowance_amount(400, cuurent_amount))
    {
      cout << "\n\n";
      cout << "Are You Sure To Do This Operation? ";
      cin >> do_operation;
      if (toupper(do_operation) == 'Y')
      {
        cuurent_amount -= 400;
        cout << "Done Successfuly ,New Balance is " << cuurent_amount << endl;
      }
      else
      {
        cout << "No Changing In Balance" << endl;
      }
    }
    else
    {
      cout << "Cant Do This Operation Balance is " << cuurent_amount << endl;
    }
    break;
  case 6:
    if (Allowance_amount(600, cuurent_amount))
    {
      cout << "\n\n";
      cout << "Are You Sure To Do This Operation? ";
      cin >> do_operation;
      if (toupper(do_operation) == 'Y')
      {
        cuurent_amount -= 600;
        cout << "Done Successfuly ,New Balance is " << cuurent_amount << endl;
      }
      else
      {
        cout << "No Changing In Balance" << endl;
      }
    }
    else
    {
      cout << "Cant Do This Operation Balance is " << cuurent_amount << endl;
    }
    break;
  case 7:
    if (Allowance_amount(800, cuurent_amount))
    {
      cout << "\n\n";
      cout << "Are You Sure To Do This Operation? ";
      cin >> do_operation;
      if (toupper(do_operation) == 'Y')
      {
        cuurent_amount -= 800;
        cout << "Done Successfuly ,New Balance is " << cuurent_amount << endl;
      }
      else
      {
        cout << "No Changing In Balance" << endl;
      }
    }
    else
    {
      cout << "Cant Do This Operation Balance is " << cuurent_amount << endl;
    }
    break;
  case 8:
    if (Allowance_amount(1000, cuurent_amount))
    {
      cout << "\n\n";
      cout << "Are You Sure To Do This Operation? ";
      cin >> do_operation;
      if (toupper(do_operation) == 'Y')
      {
        cuurent_amount -= 1000;
        cout << "Done Successfuly ,New Balance is " << cuurent_amount << endl;
      }
      else
      {
        cout << "No Changing In Balance" << endl;
      }
    }
    else
    {
      cout << "Cant Do This Operation Balance is " << cuurent_amount << endl;
    }
    break;
  case 9:
    return MainAtm();
    break;
  default:
    cout << "Invlaid Choice " << endl;
    break;
  }
  SaveClientDataByAccountNumber(currentclient.AccountNumber, ClientsFileName, data, cuurent_amount);
  LoadClientData(currentclient.AccountNumber);
}
void WithDrawScreen()
{
  cout << "============================================\n";
  cout << setw(12) << " " << "Normal WithDraw Screen\n";
  cout << "============================================\n";
}
void DepoistScreen()
{
  cout << "============================================\n";
  cout << setw(12) << " " << "Depoist Screen\n";
  cout << "============================================\n";
}
void NormalWithDraw()
{
  WithDrawScreen();
  int amount;
  int cuurent_amount = currentclient.AccountBalance;
  char choice = 'Y';
  vector<sClient> data = LoadDataFromFile(ClientsFileName);
  cout << "Enter WithDraw Amout ";
  cin >> amount;
  cin.ignore();
  if (!Allowance_amount(amount, cuurent_amount))
  {
    cout << "The Amount Excceds Your Balance , Make Another Choice!\n";
    cout << "Your Balance is " << cuurent_amount << " \n";
    cout << "Press Any Key To Continue...";
    system("pause>0");
    system("cls");
    return NormalWithDraw();
  }
  else
  {
    cout << "Are You Sure To Perform This Operation ? :";
    cin >> choice;
    if (toupper(choice) == 'Y')
    {
      cuurent_amount -= amount;
      cout << "\nDone Successfuly ,New Balance is " << cuurent_amount << endl;
    }
    else
    {
      cout << "No Changing In Balance" << endl;
    }
  }
  SaveClientDataByAccountNumber(currentclient.AccountNumber, ClientsFileName, data, cuurent_amount);
  LoadClientData(currentclient.AccountNumber);
}
void Depoist()
{
  DepoistScreen();

  int depoist;
  int cuurent_amount = currentclient.AccountBalance;
  char choice = 'Y';
  vector<sClient> data = LoadDataFromFile(ClientsFileName);

  cout << "Enter a positive Depoist Amount ";
  cin >> depoist;

  cout << "Are You Sure To Do This Transaction[y/n]? ";
  cin >> choice;
  cin.ignore();

  if (toupper(choice) == 'Y')
  {
    cuurent_amount += depoist;
    cout << "\nDone Successfuly ,New Balance is " << cuurent_amount << endl;
  }
  else
  {
    cout << "No Changing In Balance" << endl;
  }

  SaveClientDataByAccountNumber(currentclient.AccountNumber, ClientsFileName, data, cuurent_amount);
  LoadClientData(currentclient.AccountNumber);
}
void systemlogin()
{
  CheakAccount();
  system("cls");
  return MainAtm();
}
void MainAtm()
{
  AtmMenuScreen();
  ShowMenuScreen();
  UserChoice user_choice = ReadChooseMenusScreen("Choose Want Do You Want To Do[1/5]?");
  system("cls");
  switch (user_choice)
  {
  case UserChoice::logout:
    return systemlogin();
    break;
  case UserChoice::checkbalance:
    CheakBalance();
    GoToMainMenu();
    break;
  case UserChoice::qwickwithdraw:
    QuickWithDraw();
    GoToMainMenu();
    break;
  case UserChoice::normalwithdraw:
    NormalWithDraw();
    GoToMainMenu();
    break;
  case UserChoice::depoist:
    Depoist();
    GoToMainMenu();
    break;
  }
}
int main()
{
  systemlogin();
  return 0;
}