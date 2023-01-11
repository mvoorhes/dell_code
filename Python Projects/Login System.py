accounts = {"Maxwell Voorhes": "password"}

run = True
while run:
    choice = input("Create or Login? ")
    if choice == "create":
        account_name = input("Enter account name: ")
        password = input("Enter password: ")
        if account_name in accounts:
            print("Error: Account Exists")
        else:
            accounts[account_name] = password
    elif choice == "login":
        account_name = input("Enter account name: ")
        password = input("Enter password: ")
        if account_name not in accounts:
            print("Error: No account here exists")
        elif accounts[account_name] != password:
            print("Error: Entered Wrong Password")
        else:
            print("Login Successful")
    else:
        run = False
