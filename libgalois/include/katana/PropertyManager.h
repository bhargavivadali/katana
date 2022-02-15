#pragma once

#include <cstdint>
#include <string>

#include "katana/Cache.h"
#include "katana/Manager.h"
#include "katana/Time.h"

namespace katana {

/// Manager for property memory
class KATANA_EXPORT PropertyManager : public Manager {
public:
  PropertyManager();
  ~PropertyManager();
  /// Returns the coarse category of memory use
  ///   e.g., property for the property manager
  static const std::string name_;
  const std::string& Name() const override { return name_; }
  count_t FreeStandbyMemory(count_t goal) override;

  /// Client wants a property, see if we have it in the cache and if so return it and
  /// make the memory active.
  /// Returns nullptr if manager does not have it in the cache
  std::shared_ptr<arrow::Table> GetProperty(const katana::Uri& property_path);

  /// The property data has come into memory from storage.
  void PropertyLoadedActive(const std::shared_ptr<arrow::Table>& property);

  /// We are done with the property.  Put it in the cache if we have room.
  void PutProperty(
      const katana::Uri& property_path,
      const std::shared_ptr<arrow::Table>& property);

  CacheStats GetPropertyCacheStats() const { return cache_->GetStats(); }
  void LogMemoryStats(const std::string& message);
  struct Stats {
    void Log() const {
      katana::GetTracer().GetActiveSpan().Log(
          "property manager stats",
          {
              {"bytes_loaded", bytes_loaded},
              {"gb_loaded", katana::ToGB(bytes_loaded)},
          });
    }
    count_t bytes_loaded{0LL};
  };
  Stats GetStats() const { return stats; }

private:
  void MakePropertyCache();
  std::unique_ptr<PropertyCache> cache_;
  Stats stats;
};

}  // namespace katana
