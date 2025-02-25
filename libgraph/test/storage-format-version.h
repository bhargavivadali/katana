#ifndef KATANA_LIBGRAPH_STORAGEFORMATVERSION_H_
#define KATANA_LIBGRAPH_STORAGEFORMATVERSION_H_

#include "katana/Logging.h"
#include "katana/PropertyGraph.h"

katana::PropertyGraph
LoadGraph(const std::string& rdg_file) {
  KATANA_LOG_ASSERT(!rdg_file.empty());
  katana::TxnContext txn_ctx;
  auto g_res =
      katana::PropertyGraph::Make(rdg_file, &txn_ctx, katana::RDGLoadOptions());

  if (!g_res) {
    KATANA_LOG_FATAL("making result: {}", g_res.error());
  }
  katana::PropertyGraph g = std::move(*g_res.value());
  return g;
}

std::string
StoreGraph(katana::PropertyGraph* g) {
  auto uri_res = katana::URI::MakeRand("/tmp/propertyfilegraph");
  KATANA_LOG_ASSERT(uri_res);
  std::string tmp_rdg_dir(uri_res.value().path());  // path() because local
  std::string command_line;
  katana::TxnContext txn_ctx;

  // Store graph. If there is a new storage format then storing it is enough to bump the version up.
  KATANA_LOG_WARN("writing graph at temp file {}", tmp_rdg_dir);
  auto write_result = g->Write(tmp_rdg_dir, command_line, &txn_ctx);
  if (!write_result) {
    KATANA_LOG_FATAL("writing result failed: {}", write_result.error());
  }
  return tmp_rdg_dir;
}

#endif
