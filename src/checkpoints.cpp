// Copyright (c) 2009-2012 The Bitcoin developers
// Copyright (c) 2013-2079 Dr. Kimoto Chan
// Copyright (c) 2013-2017 The Floripacoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "main.h"
#include "uint256.h"

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    // How many times we expect transactions after the last checkpoint to
    // be slower. This number is a compromise, as it can't be accurate for
    // every system. When reindexing from a fast disk with a slow CPU, it
    // can be up to 20, while when downloading from a slow network with a
    // fast multicore CPU, it won't be much higher than 1.
    static const double fSigcheckVerificationFactor = 5.0;

    struct CCheckpointData {
        const MapCheckpoints *mapCheckpoints;
        int64 nTimeLastCheckpoint;
        int64 nTransactionsLastCheckpoint;
        double fTransactionsPerDay;
    };

    bool fEnabled = true;

    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
		//(	0, hashGenesisBlock)
		( 100000, uint256("0x9de061dcc57549d0e3d813afc64792faf6706b650784b81206bfa50a34b8a0a8"))
		( 200000, uint256("0xcaef7126d73e6ed0305e43e8a7a6da61c273189966a4fc05a0effd00e5d86d51"))
		( 500000, uint256("0xdc3c558c0c8ecfaad0e0569b7f0581a267326f8aca4f1062cfb69b739c7764fc"))
		(1000000, uint256("0xed1351708f770d56aaf4fdd2223e4b71e56ef149308db2d12f8d802584749dba"))
		(1500000, uint256("0xeb62720b0da89fa7f441056d7f7943efb9c2234a6688826951338c069720a38a"))
		(1600000, uint256("0x35acdbed7b532a5390e97642f387b4d4acb4b4fa133c3cb6aa91bc1394cb5a51"))
		(1700000, uint256("0x7e31d418ac0b74ec35df3c891f1b88f498de8f5ac343e3b99efebfe17fb7c3da"))
		(1800000, uint256("0x0e2fa345d10b2ab1d6472da0f36903ea2534b4c5776b9b2e630a09ab2f3737de"))
		(1837777, uint256("0x9e6a434b2cfb72b48346ef5d23ec6cdda6b4fe07989b90d2de988bb019405df6"))
    ;
    static const CCheckpointData data = {
        &mapCheckpoints,
        1384183736, // * UNIX timestamp of last checkpoint block
        365070,   	// * total number of transactions between genesis and last checkpoint
                    //   (the tx=... number in the SetBestChain debug.log lines)
        60000.0     // * estimated number of transactions per day after checkpoint
    };
	
    static MapCheckpoints mapCheckpointsTestnet = 
        boost::assign::map_list_of
		//(	0, hashGenesisBlock)
        (   2236, uint256("0xfae5f0ff729c2a296d1aa486818a35279babc40da15dba5ceef047be2c4a7a7a"))
    ;
    static const CCheckpointData dataTestnet = {
        &mapCheckpointsTestnet,
        1370212592,
        2235,
        300
    };
	
    const CCheckpointData &Checkpoints() {
        if (fTestNet)
            return dataTestnet;
        else
            return data;
    }

    bool CheckBlock(int nHeight, const uint256& hash)
    {
        if (!fEnabled)
            return true;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    // Guess how far we are in the verification process at the given block index
    double GuessVerificationProgress(CBlockIndex *pindex) {
        if (pindex==NULL)
            return 0.0;

        int64 nNow = time(NULL);

        double fWorkBefore = 0.0; // Amount of work done before pindex
        double fWorkAfter = 0.0;  // Amount of work left after pindex (estimated)
        // Work is defined as: 1.0 per transaction before the last checkoint, and
        // fSigcheckVerificationFactor per transaction after.

        const CCheckpointData &data = Checkpoints();

        if (pindex->nChainTx <= data.nTransactionsLastCheckpoint) {
            double nCheapBefore = pindex->nChainTx;
            double nCheapAfter = data.nTransactionsLastCheckpoint - pindex->nChainTx;
            double nExpensiveAfter = (nNow - data.nTimeLastCheckpoint)/86400.0*data.fTransactionsPerDay;
            fWorkBefore = nCheapBefore;
            fWorkAfter = nCheapAfter + nExpensiveAfter*fSigcheckVerificationFactor;
        } else {
            double nCheapBefore = data.nTransactionsLastCheckpoint;
            double nExpensiveBefore = pindex->nChainTx - data.nTransactionsLastCheckpoint;
            double nExpensiveAfter = (nNow - pindex->nTime)/86400.0*data.fTransactionsPerDay;
            fWorkBefore = nCheapBefore + nExpensiveBefore*fSigcheckVerificationFactor;
            fWorkAfter = nExpensiveAfter*fSigcheckVerificationFactor;
        }

        return fWorkBefore / (fWorkBefore + fWorkAfter);
    }

    int GetTotalBlocksEstimate()
    {
        if (!fEnabled)
            return 0;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        if (!fEnabled)
            return NULL;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }
}
