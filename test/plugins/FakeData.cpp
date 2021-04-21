/**
 * @file FakeDataProducer.cc FakeDataProducer class
 * implementation
 *
 * This file exists to generate the code needed to make
 * dunedaq::nwqueueadapters::fsd::FakeData usable in
 * QueueToNetwork and
 * NetworkToQueue. It has to be compiled with
 * daq_add_plugin() in the CMakeLists.txt, with a line like this:
 *
 *    daq_add_plugin( FakeData duneNetworkQueue TEST LINK_LIBRARIES nwqueueadapters )
 *
 * The generated plugin is used inside a
 * QueueToNetwork or
 * NetworkToQueue. Pass these parameters
 * to that module at init time:
 *
 *     "msg_type": "dunedaq::nwqueueadapters::fsd::FakeData",
 *     "msg_module_name": "FakeData",
 *
 * where the "msg_type" matches the argument to
 * DEFINE_DUNE_NWQUEUEADAPTERS, and the "msg_module_name" is the plugin
 * name given to daq_add_plugin
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

// This include is for the DEFINE_DUNE_NWQUEUEADAPTERS macro used below
#include "nwqueueadapters/AdapterMacros.hpp"

// These includes are for the definition of the FakeData class and the
// code to serialize it to MsgPack and JSON formats. In this case, the
// includes are all generated by moo, but they do not need to be
#include "nwqueueadapters/fsd/MsgP.hpp"
#include "nwqueueadapters/fsd/Nljs.hpp"
#include "nwqueueadapters/fsd/Structs.hpp"

DEFINE_DUNE_NWQUEUEADAPTERS(dunedaq::nwqueueadapters::fsd::FakeData)
