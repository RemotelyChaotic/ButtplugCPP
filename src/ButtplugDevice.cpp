#include "ButtplugDevice.h"

#include "buttplug_rs_ffi.pb.h"

using namespace Buttplug;
using namespace google;

Device::Device(const std::string& name, Buttplug::Client* client, Buttplug::DeviceHandle deviceHandle)
    : client(client), deviceHandle(deviceHandle), name(name)
{
    arena = std::make_unique<google::protobuf::Arena>();
}

Device::~Device() 
{
    Buttplug::FFI::FreeDevice(deviceHandle);
    deviceHandle = nullptr;   
}

void Device::sendMessage(Buttplug::DeviceMessage* msg)
{
    std::string sendBuffer;
    if(msg->SerializeToString(&sendBuffer)) {
        Buttplug::FFI::DeviceProtobufMessage(deviceHandle, (uint8_t*)sendBuffer.data(),
                                             static_cast<int>(sendBuffer.size()));
        arena->Reset(); msg = nullptr;
    }
    else {
        assert(false);
    }
}

void Device::SendLinearCmd(uint32_t durationMs, double position)
{
    auto linearCmd = protobuf::Arena::CreateMessage<Buttplug::DeviceMessage::LinearCmd>(arena.get());
    auto movement = linearCmd->add_movements();
    movement->set_index(0);
    movement->set_duration(durationMs);
    movement->set_position(position);

    auto ffi = protobuf::Arena::CreateMessage<Buttplug::DeviceMessage::FFIMessage>(arena.get());
    ffi->set_allocated_linear_cmd(linearCmd);
    auto msg = protobuf::Arena::CreateMessage<Buttplug::DeviceMessage>(arena.get());
    msg->set_allocated_message(ffi);

    sendMessage(msg);
}

void Device::SendRotateCmd(bool clockwise, double speed)
{
  auto rotateCmd = protobuf::Arena::CreateMessage<Buttplug::DeviceMessage::RotateCmd>(arena.get());
  auto rotations = rotateCmd->add_rotations();
  rotations->set_index(0);
  rotations->set_clockwise(clockwise);
  rotations->set_speed(speed);

  auto ffi = protobuf::Arena::CreateMessage<Buttplug::DeviceMessage::FFIMessage>(arena.get());
  ffi->set_allocated_rotate_cmd(rotateCmd);
  auto msg = protobuf::Arena::CreateMessage<Buttplug::DeviceMessage>(arena.get());
  msg->set_allocated_message(ffi);

  sendMessage(msg);
}

void Device::SetndStopCmd()
{
  auto stopCmd = protobuf::Arena::CreateMessage<Buttplug::DeviceMessage::StopDeviceCmd>(arena.get());

  auto ffi = protobuf::Arena::CreateMessage<Buttplug::DeviceMessage::FFIMessage>(arena.get());
  ffi->set_allocated_stop_device_cmd(stopCmd);
}

void Device::SendVibrateCmd(double speed)
{
    auto vibrateCmd = protobuf::Arena::CreateMessage<Buttplug::DeviceMessage::VibrateCmd>(arena.get());
    auto speedComp = vibrateCmd->add_speeds();
    speedComp->set_index(0);
    speedComp->set_speed(speed);

    auto ffi = protobuf::Arena::CreateMessage<Buttplug::DeviceMessage::FFIMessage>(arena.get());
    ffi->set_allocated_vibrate_cmd(vibrateCmd);
    auto msg = protobuf::Arena::CreateMessage<Buttplug::DeviceMessage>(arena.get());
    msg->set_allocated_message(ffi);

    sendMessage(msg);
}
