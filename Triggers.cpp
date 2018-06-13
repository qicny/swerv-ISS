#include "Triggers.hpp"


using namespace WdRiscv;


template <typename URV>
bool
Triggers<URV>::readData1(URV trigger, URV& value) const
{
  if (trigger >= triggers_.size())
    return false;

  value = triggers_.at(trigger).readData1();
  return true;
}


template <typename URV>
bool
Triggers<URV>::readData2(URV trigger, URV& value) const
{
  if (trigger >= triggers_.size())
    return false;

  value = triggers_.at(trigger).readData2();
  return true;
}


template <typename URV>
bool
Triggers<URV>::readData3(URV trigger, URV& value) const
{
  if (trigger >= triggers_.size())
    return false;

  return false;
}


template <typename URV>
bool
Triggers<URV>::writeData1(URV trigger, URV value)
{
  if (trigger >= triggers_.size())
    return false;

  triggers_.at(trigger).writeData1(value);
  return true;
}


template <typename URV>
bool
Triggers<URV>::writeData2(URV trigger, URV value)
{
  if (trigger >= triggers_.size())
    return false;

  triggers_.at(trigger).writeData2(value);
  return true;
}


template <typename URV>
bool
Triggers<URV>::writeData3(URV trigger, URV value)
{
  if (trigger >= triggers_.size())
    return false;

  return false;
}


template <typename URV>
bool
Triggers<URV>::reset(URV trigger, URV data1, URV data2, URV mask1, URV mask2)
{
  if (trigger >= triggers_.size())
    return false;

  triggers_.at(trigger).data1_.value_ = data1;
  triggers_.at(trigger).data2_ = data2;

  triggers_.at(trigger).data1Mask_ = mask1;
  triggers_.at(trigger).data2Mask_ = mask2;

  return true;
}


template <typename URV>
bool
Trigger<URV>::matchLdStAddr(URV address, TriggerTiming timing, bool isLoad) const
{
  if (Type(data1_.data1_.type_) != Type::Address)
    return false;  // Not an address trigger.

  if (not data1_.mcontrol_.m_)
    return false;  // Not enabled;

  bool isStore = not isLoad;
  const Mcontrol<URV>& ctl = data1_.mcontrol_;

  if (TriggerTiming(ctl.timing_) == timing and
      Select(ctl.select_) == Select::MatchAddress and
      ((isLoad and ctl.load_) or (isStore and ctl.store_)))
    {
      switch (Match(data1_.mcontrol_.match_))
	{
	case Match::Equal: return address == data2_;
	case Match::Masked: return false; // FIX
	case Match::GE: return address >= data2_;
	case Match::LT: return address < data2_;
	case Match::MaskHighEqualLow: return false; // FIX
	case Match::MaskLowEqualHigh: return false; // FIX
	}
    }
  return false;
}


template <typename URV>
bool
Trigger<URV>::matchLdStData(URV value, TriggerTiming timing, bool isLoad) const
{
  if (Type(data1_.data1_.type_) != Type::Address)
    return false;  // Not an address trigger.

  if (not data1_.mcontrol_.m_)
    return false;  // Not enabled;

  bool isStore = not isLoad;
  const Mcontrol<URV>& ctl = data1_.mcontrol_;

  if (TriggerTiming(ctl.timing_) == timing and
      Select(ctl.select_) == Select::MatchData and
      ((isLoad and ctl.load_) or (isStore and ctl.store_)))
    {
      switch (Match(data1_.mcontrol_.match_))
	{
	case Match::Equal: return value == data2_;
	case Match::Masked: return false; // FIX
	case Match::GE: return value >= data2_;
	case Match::LT: return value < data2_;
	case Match::MaskHighEqualLow: return false; // FIX
	case Match::MaskLowEqualHigh: return false; // FIX
	}
    }
  return false;
}


template class WdRiscv::Trigger<uint32_t>;
template class WdRiscv::Trigger<uint64_t>;

template class WdRiscv::Triggers<uint32_t>;
template class WdRiscv::Triggers<uint64_t>;
