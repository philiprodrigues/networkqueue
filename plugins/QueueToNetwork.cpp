/**
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "nwqueueadapters/QueueToNetwork.hpp"

#include <chrono>
#include <ers/Severity.hpp>
#include <ers/ers.hpp>
#include <string>
#include <vector>

#include "appfwk/DAQModuleHelper.hpp"

#include "serialization/Serialization.hpp"

#include "nwqueueadapters/networkobjectsender/Nljs.hpp"
#include "nwqueueadapters/queuetonetwork/Nljs.hpp"

namespace dunedaq::nwqueueadapters {

QueueToNetwork::QueueToNetwork(const std::string& name)
  : appfwk::DAQModule(name)
  , thread_(std::bind(&QueueToNetwork::do_work, this, std::placeholders::_1))
  , impl_(nullptr)
{

  register_command("conf", &QueueToNetwork::do_configure);
  register_command("start", &QueueToNetwork::do_start);
  register_command("stop", &QueueToNetwork::do_stop);
}

void
QueueToNetwork::init(const data_t& init_data)
{
  queue_instance_ = dunedaq::appfwk::queue_index(init_data, {"input"})["input"].inst;
}

void
QueueToNetwork::do_configure(const data_t& config_data)
{
  auto conf = config_data.get<dunedaq::nwqueueadapters::queuetonetwork::Conf>();
  auto sender_conf = conf.sender_config.get<dunedaq::nwqueueadapters::networkobjectsender::Conf>();
  message_type_name_=conf.msg_type;

  impl_ = makeQueueToNetworkBase(conf.msg_module_name, message_type_name_, queue_instance_, sender_conf);
  if (impl_.get() == nullptr) {
    throw std::runtime_error("No QToN for requested msg_type");
  }
}

void
QueueToNetwork::do_start(const data_t& /*args*/)
{
  thread_.start_working_thread();
}

void
QueueToNetwork::do_stop(const data_t& /*args*/)
{
  thread_.stop_working_thread();
}

void
QueueToNetwork::do_work(std::atomic<bool>& running_flag)
{
  while (running_flag.load()) {
    // TODO: Proper handling of "stop"
    try {
      impl_->get();
    } catch (const dunedaq::appfwk::QueueTimeoutExpired&) {
      continue;
    } catch (const dunedaq::ipm::SendTimeoutExpired& e) {
      ers::warning(QueueToNetworkSendTimeout(ERS_HERE, message_type_name_, queue_instance_, e));
      continue;
    }
  }
}

DEFINE_DUNE_DAQ_MODULE(QueueToNetwork)
} // namespace dunedaq::nwqueueadapters
