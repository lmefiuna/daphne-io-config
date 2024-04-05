#include <iostream>
#include "DaphneConfigCommandBuilder.h"
#include "masks.h"

DaphneConfigCommandBuilder::DaphneConfigCommandBuilder() {}
DaphneConfigCommandBuilder::~DaphneConfigCommandBuilder() {}

std::string DaphneConfigCommandBuilder::enableChannelOffsetGain(
    const CHANNEL_NUMBER_t &channel, bool enable)
{
    std::ostringstream ss;

    ss << "CFG OFFSET CH " << channel << " GAIN ";
    ss << ((enable) ? 2 : 1);
    ss << "\r\n";

    return ss.str();
}

std::string DaphneConfigCommandBuilder::applyChannelOffsetVoltage_mV(
    const CHANNEL_NUMBER_t &channel, const int value_mVolts)
{
    std::ostringstream ss;

    ss << "WR OFFSET CH " << channel << " V " << value_mVolts << "\r\n";

    return ss.str();
}

std::string DaphneConfigCommandBuilder::applyAFEGain_V(
    const AFE_NUMBER_t &afe, const double value_Volts)
{
    std::ostringstream ss;

    ss << "WR AFE " << (int)afe << " VGAIN V ";
    ss << this->calculateAFEVGainReferenceValue(value_Volts);
    ss << "\r\n";

    return ss.str();
}

int DaphneConfigCommandBuilder::calculateAFEVGainReferenceValue(const double vgain_volts)
{
    double vGain_value = ((2.49 + 1.5) / 1.5) * vgain_volts;
    return (int)(vGain_value * 1000.0);
}

std::string DaphneConfigCommandBuilder::configureAFEReg52(
    const AFE_NUMBER_t &afe, const REG_52_PARAMS_t &reg_52_params)
{
    std::ostringstream ss;
    uint16_t reg_52_value = 0;

    this->applyAFEReg52Mask_LNAGain(reg_52_value, reg_52_params.lna_gain_db);
    this->applyAFEReg52Mask_LNAIntegrator(reg_52_value, reg_52_params.lna_integrator_enable);

    ss << "WR AFE " << (int)afe << " REG 52 V " << reg_52_value << "\r\n";

    return ss.str();
}

void DaphneConfigCommandBuilder::applyAFEReg52Mask_LNAGain(uint16_t &reg_value, const LNA_GAIN_DB_t &gain_db)
{
    uint16_t lna_gain_mask;

    switch ((int)gain_db)
    {
    case LNA_GAIN_12_DB:
        lna_gain_mask = MASK_LNA_GAIN_12DB_REG_52;
        break;
    case LNA_GAIN_18_DB:
        lna_gain_mask = MASK_LNA_GAIN_18DB_REG_52;
        break;
    case LNA_GAIN_24_DB:
        lna_gain_mask = MASK_LNA_GAIN_24DB_REG_52;
        break;
    default:
        lna_gain_mask = 0;
        break;
    }

    uint16_t eraser = MASK_ERASER_LNA_GAIN_CONTROL_REG_52;
    reg_value = this->eraseAndApplyMask(reg_value, lna_gain_mask, eraser);
}

void DaphneConfigCommandBuilder::applyAFEReg52Mask_LNAIntegrator(uint16_t &reg_value, const bool &enable)
{
    uint16_t lna_integrator_mask =
        (enable) ? MASK_LNA_INTEGRATOR_EN_REG_52 : MASK_LNA_INTEGRATOR_DIS_REG_52;

    uint16_t eraser = MASK_ERASER_LNA_INTEGRATOR_REG_52;
    reg_value = this->eraseAndApplyMask(reg_value, lna_integrator_mask, eraser);
}

std::string DaphneConfigCommandBuilder::configureAFEReg51(const AFE_NUMBER_t &afe, const REG_51_PARAMS_t &reg_51_params)
{
    std::ostringstream ss;
    uint16_t reg_51_value = 0;

    this->applyAFEReg51Mask_PGAGain(reg_51_value, reg_51_params.pga_gain_db);
    this->applyAFEReg51Mask_PGAIntegrator(reg_51_value, reg_51_params.pga_integrator_enable);

    ss << "WR AFE " << (int)afe << " REG 51 V " << reg_51_value << "\r\n";

    return ss.str();
}

void DaphneConfigCommandBuilder::applyAFEReg51Mask_PGAGain(uint16_t &reg_value, const PGA_GAIN_DB_t &gain_db)
{
    uint16_t pga_gain_mask;
    switch ((int)gain_db)
    {
    case PGA_GAIN_24_DB:
        pga_gain_mask = MASK_PGA_GAIN_24DB_CONTROL_REG_51;
        break;
    case PGA_GAIN_30_DB:
        pga_gain_mask = MASK_PGA_GAIN_30DB_CONTROL_REG_51;
        break;
    default:
        pga_gain_mask = 0;
        break;
    }

    uint16_t eraser = MASK_ERASER_PGA_GAIN_CONTROL_REG_51;
    reg_value = this->eraseAndApplyMask(reg_value, pga_gain_mask, eraser);
}

void DaphneConfigCommandBuilder::applyAFEReg51Mask_PGAIntegrator(uint16_t &reg_value, const bool &enable)
{
    uint16_t pga_integrator_mask =
        (enable) ? MASK_PGA_INTEGRATOR_EN_REG_51 : MASK_PGA_INTEGRATOR_DIS_REG_51;

    uint16_t eraser = MASK_ERASER_PGA_INTEGRATOR_REG_51;
    reg_value = this->eraseAndApplyMask(reg_value, pga_integrator_mask, eraser);
}

uint16_t DaphneConfigCommandBuilder::eraseAndApplyMask(uint16_t &reg, uint16_t &mask, uint16_t &eraser)
{
    uint16_t value_reg, eraser_;
    eraser_ = ~eraser;
    value_reg = reg & eraser_;
    value_reg = value_reg | mask;
    return value_reg;
}