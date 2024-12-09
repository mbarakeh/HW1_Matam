//
// Created by moham on 08/12/2024.
//
#include "BlockChain.h"



int BlockChainGetSize(const BlockChain &blockChain) {
    int count = 0;
    const Block *temp = blockChain.head;
    while (temp) {
        count++;
        temp = temp->next;
    }
    return count;
}

int BlockChainPersonalBalance(const BlockChain &blockChain, const string &name) {
    unsigned int ReceivedValue = 0, SendedValue = 0;
    const Block *temp = blockChain.head;
    while (temp) {
        if (temp->transaction.receiver == name) {
            ReceivedValue += temp->transaction.value;
        }
        if (temp->transaction.sender == name) {
            SendedValue += temp->transaction.value;
        }
        temp = temp->next;
    }
    return ReceivedValue - SendedValue;
}

void BlockChainAppendTransaction(BlockChain &blockChain
                                 ,unsigned int value,const string &sender,
                                 const string &receiver,
        const string &timestamp) {
    Transaction newTransaction;
    newTransaction.value = value;
    newTransaction.receiver = receiver;
    newTransaction.sender = sender;
    Block *newBlock = new Block{newTransaction, timestamp, nullptr};
    newBlock->next = blockChain.head;
    blockChain.head = newBlock;

}

void BlockChainAppendTransaction(
        BlockChain &blockChain,
        const Transaction &transaction,
        const string &timestamp
) {
    Block *newBlock = new Block{transaction, timestamp, nullptr};
    newBlock->next = blockChain.head;
    blockChain.head = newBlock;
}

BlockChain BlockChainLoad(ifstream &file) {
    string sender, receiver, timestamp ;
    unsigned int value;
    Block *head;
    Block *prev;
    if (file >> sender >> receiver >> value >> timestamp) {
        Block *newBlock = new Block{{value, sender, receiver}, timestamp, nullptr};
        head = newBlock;
        prev = head;
    }else{
        BlockChain nullChain = {nullptr, nullptr};
        return nullChain;
    }
    while (file >> sender >> receiver >> value >> timestamp) {
        Block *newBlock = new Block{{value, sender, receiver}, timestamp, nullptr};
        prev->next = newBlock;
        prev = newBlock;
    }
    BlockChain newBlockChain = {head, prev};
    return newBlockChain;
}
void BlockChainDump(const BlockChain &blockChain, ofstream &file) {
    const Block *currentChainBlock = blockChain.head;
    int blockchainNumber = 1;

    file << "BlockChain Info:" << std::endl;

    while (currentChainBlock != nullptr) {
        file << blockchainNumber << "." << std::endl;
        file << "Sender Name: " << currentChainBlock->transaction.sender << std::endl;
        file << "Receiver Name: " << currentChainBlock->transaction.receiver << std::endl;
        file << "Transaction Value: " << currentChainBlock->transaction.value << std::endl;
        file << "Transaction timestamp: " << currentChainBlock->TransactionTime << std::endl;
        currentChainBlock = currentChainBlock->next;
        blockchainNumber++;
    }
}

void BlockChainDumpHashed(const BlockChain &blockChain, ofstream &file) {
    const Block *currentBlock = blockChain.head;
    string hashcode = TransactionHashedMessage(currentBlock->transaction);
    file << hashcode;
    currentBlock = currentBlock->next;
    while (currentBlock != nullptr) {
        file << std::endl;
        hashcode = TransactionHashedMessage(currentBlock->transaction);
        file << hashcode;
        currentBlock = currentBlock->next;
    }
}



bool BlockChainVerifyFile(const BlockChain &blockChain, std::ifstream &file) {
    const Block *current = blockChain.head;
    string  line;
    while (getline(file, line)) {
        if (current == nullptr){
            return false;
        }
        if (line != TransactionHashedMessage(current->transaction)) {
            return false;
        }
        current = current->next;
    }
    if (current != nullptr){
        return false;
    }
    return true;
}



void BlockChainCompress(BlockChain &blockChain) {
     int Sum = 0;
    Block *currentBlock = blockChain.head, *tmp = blockChain.head;

    while (currentBlock != nullptr && currentBlock->next != nullptr) {
        while (currentBlock->next != nullptr && currentBlock->transaction.receiver == currentBlock->next->transaction.receiver &&
               currentBlock->transaction.sender == currentBlock->next->transaction.sender) {
            Sum += currentBlock->transaction.value;
            if (currentBlock != tmp){
                Block *toDelete = currentBlock;
                currentBlock = currentBlock->next;
                delete toDelete;
            }else {
                currentBlock = currentBlock->next;
            }

        }
        if (Sum > 0) {
            Sum += currentBlock->transaction.value;
            tmp->transaction.value = Sum;
            tmp->next = currentBlock->next;
            Block *toDelete = currentBlock;
            delete toDelete;

            Sum = 0;
        }
        tmp = tmp->next;
        currentBlock = tmp;
    }
}
void BlockChainTransform(BlockChain& blockChain, updateFunction function){
    Block *current = blockChain.head;
    while (current != nullptr) {
        current->transaction.value = function(current->transaction.value);
    }
}

