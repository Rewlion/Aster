#include <engine/render/frame_graph/dsl.h>

NODE_BEGIN(gibs_binning_sync)
  RENAME_BUF(gibs_surfels_storage, gibs_surfels_storage_binned, BUF_STATE(SRV))
  RENAME_BUF(gibs_surfels_spatial_storage, gibs_surfels_spatial_storage_binned, BUF_STATE(SRV))
  RENAME_BUF(gibs_surfels_meta, gibs_surfels_meta_binned, BUF_STATE(SRV))
  RENAME_BUF(gibs_surfels_pool, gibs_surfels_pool_binned, BUF_STATE(UAV_RW))
  NO_EXEC()
NODE_END()

NODE_BEGIN(gibs_sync_out)
  NO_EXEC()
  ORDER_ME_BEFORE(gbuffer_resolve)
NODE_END()
