#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>

// Function to compute SHA256 hash
std::string sha256(const std::string& data) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, data.c_str(), data.size());
    SHA256_Final(hash, &sha256);

    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

// Block structure
struct Block {
    int index;
    std::string prevHash;
    std::string hash;
    std::time_t timestamp;
    std::string data;
    int nonce;

    // Constructor
    Block(int idx, const std::string& prevHash, const std::string& data)
        : index(idx), prevHash(prevHash), data(data), nonce(0) {
        timestamp = std::time(nullptr);
        hash = calculateHash();
    }

    // Hash calculation
    std::string calculateHash() const {
        std::stringstream ss;
        ss << index << prevHash << timestamp << data << nonce;
        return sha256(ss.str());
    }

    // Proof-of-Work mining
    void mineBlock(int difficulty) {
        std::string target(difficulty, '0'); // Difficulty target: leading zeros
        while (hash.substr(0, difficulty) != target) {
            nonce++;
            hash = calculateHash();
        }
        std::cout << "Block mined: " << hash << std::endl;
    }
};

// Blockchain structure
class Blockchain {
private:
    std::vector<Block> chain;
    int difficulty;

    // Get the last block in the chain
    Block getLastBlock() const {
        return chain.back();
    }

public:
    // Constructor
    Blockchain(int diff) : difficulty(diff) {
        chain.push_back(createGenesisBlock());
    }

    // Genesis block creation
    Block createGenesisBlock() {
        return Block(0, "0", "Genesis Block");
    }

    // Add a new block
    void addBlock(const std::string& data) {
        Block newBlock(chain.size(), getLastBlock().hash, data);
        newBlock.mineBlock(difficulty);
        chain.push_back(newBlock);
    }

    // Display the blockchain
    void displayChain() const {
        for (const auto& block : chain) {
            std::cout << "Index: " << block.index << std::endl;
            std::cout << "Previous Hash: " << block.prevHash << std::endl;
            std::cout << "Timestamp: " << block.timestamp << std::endl;
            std::cout << "Data: " << block.data << std::endl;
            std::cout << "Hash: " << block.hash << std::endl;
            std::cout << "Nonce: " << block.nonce << std::endl;
            std::cout << "-----------------------------" << std::endl;
        }
    }
};

int main() {
    // Create blockchain with difficulty level
    Blockchain myBlockchain(4); // Difficulty: 4 leading zeros

    std::cout << "Mining block 1..." << std::endl;
    myBlockchain.addBlock("Block 1 Data");

    std::cout << "Mining block 2..." << std::endl;
    myBlockchain.addBlock("Block 2 Data");

    std::cout << "Mining block 3..." << std::endl;
    myBlockchain.addBlock("Block 3 Data");

    // Display the blockchain
    std::cout << "\nBlockchain:" << std::endl;
    myBlockchain.displayChain();

    return 0;
}
