/*
Two-Phase Locking (2PL) is a concurrency control method used in database systems to ensure serializability of transactions. It's a locking protocol that governs how and when locks are acquired and released to maintain consistency and prevent conflicts. 

The two phases in this protocol are:

1. **Growing Phase (Lock Acquisition)**: In this phase, a transaction may acquire locks but cannot release any locks. This phase continues until the transaction reaches its "lock point", at which it has made all the locks that it needs to complete its operation.

2. **Shrinking Phase (Lock Release)**: After the lock point, the transaction enters the shrinking phase, during which it may release locks but cannot acquire any new ones. Typically, this phase occurs after the transaction has executed its final operation and it starts releasing locks, allowing other transactions to proceed.

The protocol ensures serializability (the outcome of concurrently executing transactions is the same as if they were executed one after the other) and also prevents some common issues in concurrent transactions:

- **Deadlock**: This is a situation where two or more transactions are unable to proceed because each is waiting for the other to release a resource. By enforcing an order in which locks can be acquired, 2PL can prevent deadlock.

- **Dirty Read**: This is a situation where a transaction reads data that has been written by a running transaction but not yet committed. 2PL prevents this by ensuring that a transaction holds all its locks until it is committed.

- **Non-repeatable Read**: This is a situation where a transaction reads the same record twice but gets different data each time. 2PL prevents this by ensuring that other transactions can't modify data that has been read by the current transaction until the current transaction is completed.

While 2PL can prevent these issues, it can also lower the level of concurrency in the system, because transactions often need to wait for others to release locks. As a result, there is a trade-off between the level of consistency and the level of concurrency in the system.

In the context of your previous question about `Bank::transferMoney`, 2PL is used to ensure that the transfer operation is atomic and consistent, even in the presence of concurrent transactions.
*/
void Bank::transferMoney(Account* accountA, Account* accountB, double amount) {
    accountA->changeMoney(-amount);
    accountB->changeMoney(amount);
}
/*
To make the `transferMoney` function atomic, you can use a technique called Two-Phase Locking (2PL). 2PL ensures that both deductions and additions are performed as a single atomic operation, preventing other threads from interleaving their changes in between.

Here's an example implementation using `std::lock` to acquire locks on both accounts:
*/

void Bank::transferMoney(Account* accountA, Account* accountB, double amount) {
    std::lock(accountA->mMoneyLock, accountB->mMoneyLock);  // Acquire locks in a consistent order
    std::lock_guard<std::mutex> lockA(accountA->mMoneyLock, std::adopt_lock);  // Adopt the lock for accountA
    std::lock_guard<std::mutex> lockB(accountB->mMoneyLock, std::adopt_lock);  // Adopt the lock for accountB

    accountA->changeMoney(-amount);
    accountB->changeMoney(amount);
}

/*
In this code, `std::lock` is used to acquire locks for `accountA` and `accountB`. By using `std::lock_guard` with the `std::adopt_lock` parameter, we ensure that the locks are properly released when the `std::lock_guard` objects (`lockA` and `lockB`) go out of scope.

By acquiring both locks before performing the money changes, we guarantee that no other thread can acquire either of the locks in the meantime, thus making the `changeMoney` calls on `accountA` and `accountB` atomic with respect to each other.

It's important to note that the order of acquiring locks is crucial to avoid potential deadlocks. In this example, we acquire the locks in a consistent order: first `accountA->mMoneyLock` and then `accountB->mMoneyLock`. This helps prevent deadlocks where two threads each acquire a lock in a different order and then try to acquire the other lock, causing them to wait indefinitely.

Make sure to adjust the code according to your specific implementation of `Account` and the associated mutexes or locks.
*/