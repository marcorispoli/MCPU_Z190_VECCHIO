TEMPLATE = subdirs

TEST_SOURCE = $${PWD}/../../SOURCE

SUBDIRS += \
    $${TEST_SOURCE}/CanDriverInterface \
    $${TEST_SOURCE}/TitanCanDriverInterface \
    $${TEST_SOURCE}/CommandProtocol \
    $${TEST_SOURCE}/CanOpenInterface \
    $${TEST_SOURCE}/ErrorTable \
    $${TEST_SOURCE}/awsInterface \

