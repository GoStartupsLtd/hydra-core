// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"
#include "consensus/consensus.h"
#include "pow.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>
#include <iostream>

#include "chainparamsseeds.h"
#include "amount.h"
///////////////////////////////////////////// // qtum
#include <libdevcore/SHA3.h>
#include <libdevcore/RLP.h>
#include "arith_uint256.h"
/////////////////////////////////////////////

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 00 << 488804799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    genesis.hashStateRoot = uint256(h256Touint(dev::h256("b8101f72d2e3260e2a65ee2d3e815d00dc170c009d15f7a9b6dfc9ec1b4aea68"))); // qtum
    genesis.hashUTXORoot = uint256(h256Touint(dev::sha3(dev::rlp("")))); // qtum
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Sep 02, 2017 Bitcoin breaks $5,000 in latest price frenzy";
    const CScript genesisOutputScript = CScript() << ParseHex("033a537fcd935fba9f532ef78bb97154b5ea7b8dfd8b74facc36f7193e9d5b1cab") << OP_CHECKSIG;

    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.totalCoinsSupply = 1858593274150085 ; // locktrip contract coin suply
        consensus.nBlockRewardChangeInterval = 1000000; // locktrip reward changing aprox every 1 year
        consensus.blockRewardPerInterval = {10000000, 7500000, 5000000, 2500000, 1250000};
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0x0000c17ded4782ba51571d262b55d66f68972325c5c20b408a436890a44cc3b4");
        consensus.BIP65Height = 0; // 000000000000000004c2b624ed5d7756c508d90fd0da2c7c679febfa6c4735f0
        consensus.BIP66Height = 0; // 00000000000000000379eaa19dce8c9b722d46ae6a57c2f1a988119488b50931
        consensus.powLimit = uint256S("0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimit = uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 4 * 60; // 16 minutes
        consensus.nPowTargetSpacing = 32;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = true;
        consensus.fPoSNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000000001"); // qtum

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0xbfbbfc2c3be3d4e085082aff2e4e73a4e21dbf6205bc41b84b38ffac0a8bc114"); //453354

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xaf; // 175
        pchMessageStart[1] = 0xea; // 234
        pchMessageStart[2] = 0xe9; //233
        pchMessageStart[3] = 0xf7; //247
        nDefaultPort = 3338;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1535988275, 8112333, 0x1f00ffff, 1, 0 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x0000c17ded4782ba51571d262b55d66f68972325c5c20b408a436890a44cc3b4"));
        assert(genesis.hashMerkleRoot == uint256S("0xf5e84d19570e92a7d3f3d06e048ebf5cb71c55b9ef99d012edd7f7b0cd904d58"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,48);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,63);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            {
                { 0, uint256S("0x0000c17ded4782ba51571d262b55d66f68972325c5c20b408a436890a44cc3b4")},
            }
        };

        chainTxData = ChainTxData{
            // Data as of block a1bab8db27f26952ce94fff6563931943554e36fc3a23f99cc8513270d685b2c (height 92662)
            0, // * UNIX timestamp of last known number of transactions
            0, // * total number of transactions between genesis and that timestamp
                            //   (the tx=... number in the SetBestChain debug.log lines)
            0 // * estimated number of transactions per second after that timestamp
        };
        consensus.nLastPOWBlock = 15000;
        consensus.nMPoSRewardRecipients = 1;
        consensus.nFirstMPoSBlock = consensus.nLastPOWBlock + 
                                    consensus.nMPoSRewardRecipients + 
                                    COINBASE_MATURITY;

        consensus.nFixUTXOCacheHFHeight=100000;
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.totalCoinsSupply = 1858593274150085 ; // locktrip contract coin suply
        consensus.nBlockRewardChangeInterval = 1000000; // locktrip reward changing aprox every 1 year
        consensus.blockRewardPerInterval = {10000000, 7500000, 5000000, 2500000, 1250000};
        consensus.BIP34Height = 0;
        consensus.BIP34Hash = uint256S("0x00009ce775d9ccdfcb001a73edfd42471009ae8a453bdfb53a8c715bd53977f6");
        consensus.BIP65Height = 0; // 00000000007f6655f22f98e72ed80d8b06dc761d5da09df0fa1dc4be4f861eb6
        consensus.BIP66Height = 0; // 000000002104c8c45e99a8853285a3b592602a3ccde2b832481da85e9e4ba182
        consensus.powLimit = uint256S("0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimit = uint256S("0000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 4 * 60; // 16 minutes
        consensus.nPowTargetSpacing = 32;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = true;
        consensus.fPoSNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000000001"); // qtum

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x39ffa0c5924550db0e75030ff8513c3145d491dff2e17b8e3ea1cea7b4662ff0"); //1079274

        pchMessageStart[0] = 0x07; // 7
        pchMessageStart[1] = 0x13; // 19
        pchMessageStart[2] = 0x1f; // 31
        pchMessageStart[3] = 0x03; // 3

        nDefaultPort = 1338;
        nPruneAfterHeight = 1000;

       genesis = CreateGenesisBlock(1535988275, 7482752, 0x1f00ffff, 1, 0 * COIN);
       consensus.hashGenesisBlock = genesis.GetHash();
       assert(consensus.hashGenesisBlock == uint256S("0x00009ce775d9ccdfcb001a73edfd42471009ae8a453bdfb53a8c715bd53977f6"));
       assert(genesis.hashMerkleRoot == uint256S("0xf5e84d19570e92a7d3f3d06e048ebf5cb71c55b9ef99d012edd7f7b0cd904d58"));

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.emplace_back("devnet.locktrip.com", false);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,66);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,128);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;


        checkpointData = (CCheckpointData) {
            {
                {0, uint256S("0x00009ce775d9ccdfcb001a73edfd42471009ae8a453bdfb53a8c715bd53977f6")}
            }
        };

        chainTxData = ChainTxData{
            // Data as of block 493cccf2ba87ffdabd7afc0f3242c1357fdebdc0b8c7e7adc3c6dc2b1c8ca797 (height 79167)
            0,
            0,
            0
        };

        consensus.nLastPOWBlock = 15000;
        consensus.nMPoSRewardRecipients = 1;
        consensus.nFirstMPoSBlock = consensus.nLastPOWBlock +
                                    consensus.nMPoSRewardRecipients +
                                    COINBASE_MATURITY;

        consensus.nFixUTXOCacheHFHeight=84500;
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.totalCoinsSupply = 1858593274150085 ; // locktrip contract coin suply
        consensus.nBlockRewardChangeInterval = 1000000; // locktrip reward changing aprox every 1 year
        consensus.blockRewardPerInterval = {10000000, 7500000, 5000000, 2500000, 1250000};
        consensus.BIP34Height = 0; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests) // activate for locktrip
        consensus.BIP34Hash = uint256S("0x368538fb6fe495e4b03a2b104af38b82c6f0fc8d6546f98c72255ca3a06a9ecd");
        consensus.BIP65Height = 0; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 0; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 16 * 60; // 16 minutes
        consensus.nPowTargetSpacing = 2 * 64;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.fPoSNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0xc7; // 199
        pchMessageStart[1] = 0xe8; // 232
        pchMessageStart[2] = 0xe2; // 226
        pchMessageStart[3] = 0x9f; // 159
        nDefaultPort = 2338;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1535988275, 19, 0x207fffff, 1, 0 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x368538fb6fe495e4b03a2b104af38b82c6f0fc8d6546f98c72255ca3a06a9ecd"));
        assert(genesis.hashMerkleRoot == uint256S("0xf5e84d19570e92a7d3f3d06e048ebf5cb71c55b9ef99d012edd7f7b0cd904d58"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = (CCheckpointData) {
            {
                {0, uint256S("0x368538fb6fe495e4b03a2b104af38b82c6f0fc8d6546f98c72255ca3a06a9ecd")},
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        consensus.nLastPOWBlock = 15000;
        consensus.nMPoSRewardRecipients = 1;
        consensus.nFirstMPoSBlock = consensus.nLastPOWBlock +
                                    consensus.nMPoSRewardRecipients +
                                    COINBASE_MATURITY;

        consensus.nFixUTXOCacheHFHeight=0;

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,120);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,110);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};
    }
};

/**
 * Regression network parameters overwrites for unit testing
 */
class CUnitTestParams : public CRegTestParams
{
public:
    CUnitTestParams()
    {
        // Activate the the BIPs for regtest as in Bitcoin
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)

        // LOC have 500 blocks of maturity, increased values for regtest in unit tests in order to correspond with it
		consensus.nBlockRewardChangeInterval = 750;
        consensus.blockRewardPerInterval = {10000000, 7500000, 5000000, 2500000, 1250000};
        consensus.totalCoinsSupply = 1858593274150085 ; // locktrip contract coin suply
        consensus.nRuleChangeActivationThreshold = 558; // 75% for testchains
        consensus.nMinerConfirmationWindow = 744; // Faster than normal for regtest (744 instead of 2016)
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    else if (chain == CBaseChainParams::UNITTEST)
        return std::unique_ptr<CChainParams>(new CUnitTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
//
//    std::unique_ptr<CChainParams> testChainParams = std::unique_ptr<CChainParams>(new CMainParams());
//    std::cout<<"Po4vame" << std::endl;
//    auto genesis = CreateGenesisBlock(1535988275, 8109746, 0x1f00ffff, 1, 0 * COIN);;
//
//    LogPrintf("Started working \n");
//    while (!CheckProofOfWork(genesis.GetHash(), genesis.nBits, ((CChainParams)*testChainParams).GetConsensus(), false)) {
//        ++genesis.nNonce;
//
//       // std::cout << "Working Bitch " << genesis.nNonce << "::";
//
//    }
//
//    std::cout << "DOne " << genesis.nNonce << "Hash block" << genesis.GetHash().ToString()<< std::endl;
//
//    LogPrintf("nNonce : %s \n", genesis.nNonce);
//
//    LogPrintf("Genesis : %s\n",genesis.GetHash().ToString());
//
//    testChainParams = std::unique_ptr<CChainParams>(new CTestNetParams());
//    std::cout<<"Po4vame" << std::endl;
//    genesis = CreateGenesisBlock(1535988275, 7359562, 0x1f00ffff, 1, 0 * COIN);
//
//    LogPrintf("Started working \n");
//    while (!CheckProofOfWork(genesis.GetHash(), genesis.nBits, ((CChainParams)*testChainParams).GetConsensus(), false)) {
//        ++genesis.nNonce;
//
//       // std::cout << "Working Bitch " << genesis.nNonce << "::";
//
//    }
//
//    std::cout << "DOne " << genesis.nNonce << "Hash block" << genesis.GetHash().ToString()<< std::endl;
//
//    LogPrintf("nNonce : %s \n", genesis.nNonce);
//
//    LogPrintf("Genesis : %s\n",genesis.GetHash().ToString());
//
//    testChainParams = std::unique_ptr<CChainParams>(new CRegTestParams());
//    std::cout<<"Po4vame" << std::endl;
//    genesis = CreateGenesisBlock(1535988275, 18, 0x207fffff, 1, 0 * COIN);
//
//    LogPrintf("Started working \n");
//    while (!CheckProofOfWork(genesis.GetHash(), genesis.nBits, ((CChainParams)*testChainParams).GetConsensus(), false)) {
//        ++genesis.nNonce;
//
//     //   std::cout << "Working Bitch " << genesis.nNonce << "::";
//
//    }
//
//    std::cout << "DOne " << genesis.nNonce << "Hash block" << genesis.GetHash().ToString()<< std::endl;
//
//    LogPrintf("nNonce : %s \n", genesis.nNonce);
//
//    LogPrintf("Genesis : %s\n",genesis.GetHash().ToString());
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}
