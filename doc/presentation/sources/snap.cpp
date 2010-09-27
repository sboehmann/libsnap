Snap *snap = new Snap();
snap->setErrorDetectionMode( EDM_CRC32 );
snap->setAcknowledgementEnabled( true );

std::vector<uint8_t> data = readDataSomewhere();
std::vector<uint8_t> result = snap->encode( data );

//...