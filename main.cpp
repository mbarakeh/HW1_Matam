
#include <iostream>
#include <string>
#include <fstream>
#include "BlockChain.h"
#include "Utilities.h"

using std::string;
using std::ifstream;
using std::ofstream;

void destroyList(Block *head) {
    while (head) {
        Block *toDelete = head;
        head = head->next;
        delete toDelete;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cout << getErrorMessage() << std::endl;
        return 0;
    }
    string op = argv[1];
    string source = argv[2];
    string target = argv[3];
    std::ifstream sources;
    sources.open(source);
    BlockChain blockchain = BlockChainLoad(sources);
    if (op == "format") {
        std::ofstream targets;
        targets.open(target);
        BlockChainDump(blockchain, targets);
        targets.close();
    } else {
        if (op == "hash") {
            std::ofstream targets;
            targets.open(target);
            BlockChainDumpHashed(blockchain, targets);
            targets.close();
        } else {
            if (op == "compress") {
                std::ofstream targets;
                targets.open(target);
                BlockChainCompress(blockchain);
                BlockChainDump(blockchain, targets);
                targets.close();
            } else {
                if (op == "verify") {
                    std::ifstream targets;
                    targets.open(target);
                    if (BlockChainVerifyFile(blockchain, targets)) {
                        std::cout << "Verification passed" << std::endl;
                    } else {
                        std::cout << "Verification failed" << std::endl;

                    }
                    targets.close();
                } else {
                    std::cout << getErrorMessage() << std::endl;
                }
            }
        }
    }
    sources.close();
    destroyList(blockchain.head);
    return 0;
}
