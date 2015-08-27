#include "Memory.h"

#include <memory/MemoryBlockOperations.h>

#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <sstream>
std::string intToString(int i) {
  std::stringstream out;
  out << i;
  return out.str();
}

Memory::Memory(bool use_shared_memory, MemoryOwner::Owner owner, int team_num, int player_num, bool server):
  use_shared_memory_(use_shared_memory),
  owner_(owner),
  vision_lock_(NULL),
  motion_vision_lock_(NULL),
  suffix_("TEAM" + intToString(team_num) + "PLAYER" + intToString(player_num))
{
  if (use_shared_memory_) {
    shared_memory_ = new SharedMemory(suffix_,server);
    private_memory_ = NULL;
  } else {
    shared_memory_ = NULL;
    private_memory_ = new PrivateMemory();
  }
}

Memory::Memory(const Memory &old):
  vision_lock_(NULL),
  motion_vision_lock_(NULL),
  suffix_(old.suffix_)
{
  use_shared_memory_ = old.use_shared_memory_;
  if (use_shared_memory_) {
    std::cout << "CAN'T HANDLE COPYING SHARED MEMORY AT THIS TIME" << std::endl;
    exit(1);
  }
  private_memory_ = new PrivateMemory(*old.private_memory_);
  shared_memory_ = NULL;
  owner_ = old.owner_;
}

Memory::~Memory() {
  if (shared_memory_ != NULL) {
    delete shared_memory_;
    shared_memory_ = NULL;
  }
  if (private_memory_ != NULL) {
    delete private_memory_;
    private_memory_ = NULL;
  }
}

Memory& Memory::operator=(const Memory &old) {
  if (shared_memory_ != NULL) {
    delete shared_memory_;
    shared_memory_ = NULL;
  }
  if (private_memory_ != NULL) {
    delete private_memory_;
    private_memory_ = NULL;
  }
  
  use_shared_memory_ = old.use_shared_memory_;
  if (use_shared_memory_) {
    std::cout << "CAN'T HANDLE COPYING SHARED MEMORY AT THIS TIME" << std::endl;
    exit(1);
  }
  private_memory_ = new PrivateMemory(*old.private_memory_);
  shared_memory_ = NULL;
  owner_ = old.owner_;
  return *this;
}


MemoryBlock* Memory::getBlockPtr(const std::string &name, MemoryOwner::Owner expect_owner) {
  MemoryBlock *ptr;
  if (use_shared_memory_)
    ptr = shared_memory_->getBlockPtr(name);
  else
    ptr = private_memory_->getBlockPtr(name);

  if (ptr != NULL) {
    if (expect_owner == MemoryOwner::UNKNOWN)
      expect_owner = owner_;
    ptr->checkOwner(name,expect_owner);
  }
  return ptr;
}

const MemoryBlock* Memory::getBlockPtr(const std::string &name, MemoryOwner::Owner expect_owner) const {
  MemoryBlock *ptr;
  if (use_shared_memory_)
    ptr = shared_memory_->getBlockPtr(name);
  else
    ptr = private_memory_->getBlockPtr(name);

  if (ptr != NULL) {
    if (expect_owner == MemoryOwner::UNKNOWN)
      expect_owner = owner_;
    ptr->checkOwner(name,expect_owner);
  }
  return ptr;
}

void Memory::getBlockNames(std::vector<std::string> &module_names, bool only_log) const {
  if (use_shared_memory_)
    shared_memory_->getBlockNames(module_names,only_log,owner_);
  else
    private_memory_->getBlockNames(module_names,only_log,owner_);
}

void Memory::setBlockLogging(const std::string &name, bool log_block) {
  MemoryBlock *block = getBlockPtr(name,MemoryOwner::UNKNOWN);
  if (block != NULL)
    block->log_block = log_block;
}

MemoryBlock* Memory::getBlockPtrByName(const std::string &name){
  return getBlockPtr(name,MemoryOwner::UNKNOWN);
}

const MemoryBlock* Memory::getBlockPtrByName(const std::string &name) const {
  return getBlockPtr(name,MemoryOwner::UNKNOWN);
}

bool Memory::addBlockByName(const std::string &name, MemoryOwner::Owner owner) {
  if (owner == MemoryOwner::UNKNOWN) {
    owner = owner_;
  }
  temp_add_owner_ = owner;
  ADD_MEMORY_BLOCK(name,addBlock);
}
