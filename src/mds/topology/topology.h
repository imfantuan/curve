/*
 * Project: curve
 * Created Date: Fri Aug 17 2018
 * Author: xuchaojie
 * Copyright (c) 2018 netease
 */
#ifndef CURVE_SRC_MDS_TOPOLOGY_TOPOLOGY_H_
#define CURVE_SRC_MDS_TOPOLOGY_TOPOLOGY_H_

#include <unordered_map>
#include <string>
#include <list>
#include <vector>
#include <map>

#include "proto/topology.pb.h"
#include "src/mds/common/mds_define.h"
#include "src/mds/topology/topology_item.h"
#include "src/mds/topology/topology_id_generator.h"
#include "src/mds/topology/topology_token_generator.h"
#include "src/mds/topology/topology_storge.h"
#include "src/common/rw_lock.h"

using ::curve::common::RWLock;
using ::curve::common::ReadLockGuard;
using ::curve::common::WriteLockGuard;

namespace curve {
namespace mds {
namespace topology {

struct TopologyOption {
    uint32_t ChunkServerStateUpdateSec;
};

class Topology {
 public:
    Topology() {}
    virtual ~Topology() {}
    virtual int init() = 0;

    virtual PoolIdType AllocateLogicalPoolId() = 0;
    virtual PoolIdType AllocatePhysicalPoolId() = 0;
    virtual ZoneIdType AllocateZoneId() = 0;
    virtual ServerIdType AllocateServerId() = 0;
    virtual ChunkServerIdType AllocateChunkServerId() = 0;
    virtual CopySetIdType AllocateCopySetId(PoolIdType logicalPoolId) = 0;

    virtual std::string AllocateToken() = 0;

    virtual int AddLogicalPool(const LogicalPool &data) = 0;
    virtual int AddPhysicalPool(const PhysicalPool &data) = 0;
    virtual int AddZone(const Zone &data) = 0;
    virtual int AddServer(const Server &data) = 0;
    virtual int AddChunkServer(const ChunkServer &data) = 0;
    virtual int AddCopySet(const CopySetInfo &data) = 0;

    virtual int RemoveLogicalPool(PoolIdType id) = 0;
    virtual int RemovePhysicalPool(PoolIdType id) = 0;
    virtual int RemoveZone(ZoneIdType id) = 0;
    virtual int RemoveServer(ServerIdType id) = 0;
    virtual int RemoveChunkServer(ChunkServerIdType id) = 0;
    virtual int RemoveCopySet(CopySetKey key) = 0;

    virtual int UpdateLogicalPool(const LogicalPool &data) = 0;
    virtual int UpdatePhysicalPool(const PhysicalPool &data) = 0;
    virtual int UpdateZone(const Zone &data) = 0;
    virtual int UpdateServer(const Server &data) = 0;
    // 更新内存并持久化全部数据
    virtual int UpdateChunkServer(const ChunkServer &data) = 0;
    // 更新内存，定期持久化数据
    virtual int UpdateChunkServerState(const ChunkServerState &state,
                                       ChunkServerIdType id) = 0;
    virtual int UpdateCopySet(const CopySetInfo &data) = 0;

    virtual PoolIdType
        FindLogicalPool(const std::string &logicalPoolName,
                        const std::string &physicalPoolName) const = 0;
    virtual PoolIdType FindPhysicalPool(
        const std::string &physicalPoolName) const = 0;
    virtual ZoneIdType FindZone(const std::string &zoneName,
                                const std::string &physicalPoolName) const = 0;
    virtual ZoneIdType FindZone(const std::string &zoneName,
                                PoolIdType physicalpoolid) const = 0;
    virtual ServerIdType FindServerByHostName(
        const std::string &hostName) const = 0;
    virtual ServerIdType FindServerByHostIpPort(
        const std::string &hostIp, uint32_t port) const = 0;
    virtual ChunkServerIdType FindChunkServer(const std::string &hostIp,
                                              uint32_t port) const = 0;

    virtual bool GetLogicalPool(PoolIdType poolId,
                                LogicalPool *out) const = 0;
    virtual bool GetPhysicalPool(PoolIdType poolId,
                                 PhysicalPool *out) const = 0;
    virtual bool GetZone(ZoneIdType zoneId,
                         Zone *out) const = 0;
    virtual bool GetServer(ServerIdType serverId,
                           Server *out) const = 0;
    virtual bool GetChunkServer(ChunkServerIdType chunkserverId,
                                ChunkServer *out) const = 0;

    virtual bool GetCopySet(CopySetKey key, CopySetInfo *out) const = 0;

    virtual bool GetLogicalPool(const std::string &logicalPoolName,
                                const std::string &physicalPoolName,
                                LogicalPool *out) const = 0;

    virtual bool GetPhysicalPool(const std::string &physicalPoolName,
                                 PhysicalPool *out) const = 0;

    virtual bool GetZone(const std::string &zoneName,
                         const std::string &physicalPoolName,
                         Zone *out) const = 0;

    virtual bool GetZone(const std::string &zoneName,
                         PoolIdType physicalPoolId,
                         Zone *out) const = 0;

    virtual bool GetServerByHostName(const std::string &hostName,
                                     Server *out) const = 0;

    virtual bool GetServerByHostIpPort(const std::string &hostIp,
                                   uint32_t port,
                                   Server *out) const = 0;

    virtual bool GetChunkServer(const std::string &hostIp,
                                uint32_t port,
                                ChunkServer *out) const = 0;

    virtual std::list<ChunkServerIdType> GetChunkServerInCluster() const = 0;
    virtual std::list<ServerIdType> GetServerInCluster() const = 0;
    virtual std::list<ZoneIdType> GetZoneInCluster() const = 0;
    virtual std::list<PoolIdType> GetPhysicalPoolInCluster() const = 0;
    virtual std::list<PoolIdType> GetLogicalPoolInCluster() const = 0;

    virtual std::list<ChunkServerIdType> GetChunkServerInServer(
        ServerIdType id) const = 0;
    virtual std::list<ChunkServerIdType> GetChunkServerInZone(
        ZoneIdType id) const = 0;
    virtual std::list<ChunkServerIdType> GetChunkServerInPhysicalPool(
        PoolIdType id) const = 0;

    virtual std::list<ServerIdType> GetServerInZone(ZoneIdType id) const = 0;
    virtual std::list<ServerIdType> GetServerInPhysicalPool(
        PoolIdType id) const = 0;

    virtual std::list<ZoneIdType> GetZoneInPhysicalPool(
        PoolIdType id) const = 0;
    virtual std::list<PoolIdType> GetLogicalPoolInPhysicalPool(
        PoolIdType id) const = 0;

    virtual std::list<ChunkServerIdType> GetChunkServerInLogicalPool(
        PoolIdType id) const = 0;
    virtual std::list<ServerIdType> GetServerInLogicalPool(
        PoolIdType id) const = 0;
    virtual std::list<ZoneIdType> GetZoneInLogicalPool(PoolIdType id) const = 0;

    virtual std::vector<CopySetIdType> GetCopySetsInLogicalPool(
        PoolIdType logicalPoolId) const = 0;

    virtual std::vector<CopySetKey> GetCopySetsInCluster() const = 0;

    virtual std::vector<CopySetKey>
    GetCopySetsInChunkServer(ChunkServerIdType id) const = 0;
};
class TopologyImpl : public Topology {
 public:
    TopologyImpl(std::shared_ptr<TopologyIdGenerator> idGenerator,
                 std::shared_ptr<TopologyTokenGenerator> tokenGenerator,
                 std::shared_ptr<TopologyStorage> storage)
        : idGenerator_(idGenerator),
          tokenGenerator_(tokenGenerator),
          storage_(storage) {
    }

    ~TopologyImpl() {}

    int init();

    PoolIdType AllocateLogicalPoolId() override;
    PoolIdType AllocatePhysicalPoolId() override;
    ZoneIdType AllocateZoneId() override;
    ServerIdType AllocateServerId() override;
    ChunkServerIdType AllocateChunkServerId() override;
    CopySetIdType AllocateCopySetId(PoolIdType logicalPoolId) override;

    std::string AllocateToken() override;

    int AddLogicalPool(const LogicalPool &data) override;
    int AddPhysicalPool(const PhysicalPool &data) override;
    int AddZone(const Zone &data) override;
    int AddServer(const Server &data) override;
    int AddChunkServer(const ChunkServer &data) override;
    int AddCopySet(const CopySetInfo &data) override;

    int RemoveLogicalPool(PoolIdType id) override;
    int RemovePhysicalPool(PoolIdType id) override;
    int RemoveZone(ZoneIdType id) override;
    int RemoveServer(ServerIdType id) override;
    int RemoveChunkServer(ChunkServerIdType id) override;
    int RemoveCopySet(CopySetKey key) override;

    int UpdateLogicalPool(const LogicalPool &data) override;
    int UpdatePhysicalPool(const PhysicalPool &data) override;
    int UpdateZone(const Zone &data) override;
    int UpdateServer(const Server &data) override;
    // 更新内存并持久化全部数据
    int UpdateChunkServer(const ChunkServer &data) override;
    // 更新内存，定期持久化数据
    int UpdateChunkServerState(const ChunkServerState &state,
                               ChunkServerIdType id) override;
    int UpdateCopySet(const CopySetInfo &data) override;

    PoolIdType FindLogicalPool(const std::string &logicalPoolName,
        const std::string &physicalPoolName) const override;
    PoolIdType FindPhysicalPool(
        const std::string &physicalPoolName) const override;
    ZoneIdType FindZone(const std::string &zoneName,
        const std::string &physicalPoolName) const override;
    ZoneIdType FindZone(const std::string &zoneName,
        PoolIdType physicalpoolid) const override;
    ServerIdType FindServerByHostName(
        const std::string &hostName) const override;
    ServerIdType FindServerByHostIpPort(
        const std::string &hostIp, uint32_t port) const override;
    ChunkServerIdType FindChunkServer(const std::string &hostIp,
                                      uint32_t port) const override;

    bool GetLogicalPool(PoolIdType poolId, LogicalPool *out) const override;
    bool GetPhysicalPool(PoolIdType poolId, PhysicalPool *out) const override;
    bool GetZone(ZoneIdType zoneId, Zone *out) const override;
    bool GetServer(ServerIdType serverId, Server *out) const override;
    bool GetChunkServer(ChunkServerIdType chunkserverId,
                        ChunkServer *out) const override;

    bool GetCopySet(CopySetKey key, CopySetInfo *out) const override;

    bool GetLogicalPool(const std::string &logicalPoolName,
                        const std::string &physicalPoolName,
                        LogicalPool *out) const override {
        return GetLogicalPool(
            FindLogicalPool(logicalPoolName, physicalPoolName), out);
    }
    bool GetPhysicalPool(const std::string &physicalPoolName,
                         PhysicalPool *out) const override {
        return GetPhysicalPool(FindPhysicalPool(physicalPoolName), out);
    }
    bool GetZone(const std::string &zoneName,
                 const std::string &physicalPoolName,
                 Zone *out) const override {
        return GetZone(FindZone(zoneName, physicalPoolName), out);
    }
    bool GetZone(const std::string &zoneName,
                 PoolIdType physicalPoolId,
                 Zone *out) const override {
        return GetZone(FindZone(zoneName, physicalPoolId), out);
    }
    bool GetServerByHostName(const std::string &hostName,
                             Server *out) const override {
        return GetServer(FindServerByHostName(hostName), out);
    }
    bool GetServerByHostIpPort(const std::string &hostIp,
                           uint32_t port,
                           Server *out) const override {
        return GetServer(FindServerByHostIpPort(hostIp, port), out);
    }
    bool GetChunkServer(const std::string &hostIp,
                        uint32_t port,
                        ChunkServer *out) const override {
        return GetChunkServer(FindChunkServer(hostIp, port), out);
    }

    std::list<ChunkServerIdType> GetChunkServerInCluster() const override;
    std::list<ServerIdType> GetServerInCluster() const override;
    std::list<ZoneIdType> GetZoneInCluster() const override;
    std::list<PoolIdType> GetPhysicalPoolInCluster() const override;
    std::list<PoolIdType> GetLogicalPoolInCluster() const override;

    std::list<ChunkServerIdType>
        GetChunkServerInServer(ServerIdType id) const override;
    std::list<ChunkServerIdType>
        GetChunkServerInZone(ZoneIdType id) const override;
    std::list<ChunkServerIdType>
        GetChunkServerInPhysicalPool(PoolIdType id) const override;
    std::list<ServerIdType>
        GetServerInZone(ZoneIdType id) const override;
    std::list<ServerIdType>
        GetServerInPhysicalPool(PoolIdType id) const override;

    std::list<ZoneIdType>
        GetZoneInPhysicalPool(PoolIdType id) const override;
    std::list<PoolIdType>
        GetLogicalPoolInPhysicalPool(PoolIdType id) const override;

    std::list<ChunkServerIdType> GetChunkServerInLogicalPool(
        PoolIdType id) const override;
    std::list<ServerIdType>
        GetServerInLogicalPool(PoolIdType id) const override;
    std::list<ZoneIdType>
        GetZoneInLogicalPool(PoolIdType id) const override;

    std::vector<CopySetIdType> GetCopySetsInLogicalPool(
        PoolIdType logicalPoolId) const override;
    std::vector<CopySetKey> GetCopySetsInCluster() const override;
    std::vector<CopySetKey> GetCopySetsInChunkServer(
        ChunkServerIdType id) const override;

 private:
    int CleanInvalidLogicalPoolAndCopyset();

 private:
    std::unordered_map<PoolIdType, LogicalPool> logicalPoolMap_;
    std::unordered_map<PoolIdType, PhysicalPool> physicalPoolMap_;
    std::unordered_map<ZoneIdType, Zone> zoneMap_;
    std::unordered_map<ServerIdType, Server> serverMap_;
    std::unordered_map<ChunkServerIdType, ChunkServer> chunkServerMap_;

    std::map<CopySetKey, CopySetInfo> copySetMap_;

    std::shared_ptr<TopologyIdGenerator> idGenerator_;
    std::shared_ptr<TopologyTokenGenerator> tokenGenerator_;
    std::shared_ptr<TopologyStorage> storage_;

    //以如下声明的顺序获取锁，防止死锁
    mutable curve::common::RWLock logicalPoolMutex_;
    mutable curve::common::RWLock physicalPoolMutex_;
    mutable curve::common::RWLock zoneMutex_;
    mutable curve::common::RWLock serverMutex_;
    mutable curve::common::RWLock chunkServerMutex_;
    mutable curve::common::RWLock copySetMutex_;

    TopologyOption option_;
};

}  // namespace topology
}  // namespace mds
}  // namespace curve


#endif  // CURVE_SRC_MDS_TOPOLOGY_TOPOLOGY_H_