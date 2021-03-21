#include <iostream>

struct BankAccount {
  int balance = 0;
  int overdraft_limit = -500;

  void deposit(int amount) {
    balance += amount;
    std::cout << "deposit: " << amount << ", now is: " << balance << std::endl;
  }

  void withdraw(int amount) {
    if (balance - amount >= overdraft_limit) {
      balance -= amount;
      std::cout << "withdraw: " << amount << ", now is: " << balance << std::endl;
    }
  }
};

struct Command {
  virtual void call() const = 0;
};

struct BankAccountCommand : Command {
  BankAccount& account;
  enum Action { deposit, withdraw } action;
  int amount;

  BankAccountCommand(BankAccount& account,
        const Action action,
        const int amount)
      : account(account), action(action), amount(amount) { }

  void call() const override {
    switch (action) {
      case deposit:
        account.deposit(amount);
        break;
      case withdraw:
        account.withdraw(amount);
        break;
    }
  }
};

int main() {
  BankAccount ba;
  BankAccountCommand cmd{ ba, BankAccountCommand::deposit, 100 };
  cmd.call();
}
