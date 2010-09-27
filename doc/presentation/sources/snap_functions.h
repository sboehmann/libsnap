void snap_init(snap_t *ptr);
void snap_reset(snap_t *ptr);

size_t snap_get_error_detection_mode_size(snap_t *ptr);
SnapErrorDetectionMode snap_get_error_detection_mode(snap_t *ptr);
void snap_set_error_detection_mode(snap_t *ptr, SnapErrorDetectionMode edm);

size_t snap_get_local_address_size(snap_t *ptr);
int32_t snap_get_local_address(snap_t *ptr);
void snap_set_local_address(snap_t *ptr, int32_t address);

size_t snap_get_peer_address_size(snap_t *ptr);
int32_t snap_get_peer_address(snap_t *ptr);
void snap_set_peer_address(snap_t *ptr, int32_t address);

//...