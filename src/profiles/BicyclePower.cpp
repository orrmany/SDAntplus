#include "BicyclePower.h"
#include "util.h"

PWRPage10::PWRPage10() :
    pwr_event_count(0),
    pedal_pwr(0),
    instant_cadence(0),
    accumulated_pwr(0),
    instant_pwr(0)
{}

void PWRPage10::Decode(uint8_t const* buffer)
{
    ant_pwr_page10_data_layout_t const* p_incoming_data = (ant_pwr_page10_data_layout_t *)buffer;
    pwr_event_count = p_incoming_data->pwr_event_count;
    pedal_pwr = p_incoming_data->pedal_pwr;
    instant_cadence = p_incoming_data->instant_cadence;
    accumulated_pwr = (uint16_t)((p_incoming_data->accumulated_pwr_msb << 8) + p_incoming_data->accumulated_pwr_lsb);
    instant_pwr = (uint16_t)((p_incoming_data->instant_pwr_msb << 8) + p_incoming_data->instant_pwr_lsb);
}

void PWRPage10::Encode(uint8_t* buffer)
{
    ant_pwr_page10_data_layout_t * p_outgoing_data = (ant_pwr_page10_data_layout_t *)buffer;
    p_outgoing_data->pwr_event_count = pwr_event_count;
    p_outgoing_data->pedal_pwr = pedal_pwr;
    p_outgoing_data->instant_cadence = instant_cadence;
    p_outgoing_data->accumulated_pwr_lsb = (uint8_t)(accumulated_pwr & UINT8_MAX);
    p_outgoing_data->accumulated_pwr_msb = (uint8_t)((accumulated_pwr >> 8) & UINT8_MAX);
    p_outgoing_data->instant_pwr_lsb = (uint8_t)(instant_pwr & UINT8_MAX);
    p_outgoing_data->instant_pwr_msb = (uint8_t)((instant_pwr >> 8) & UINT8_MAX);
    //Serial.printf("Event: %d\tPedal PWR: %d\tInst Cad: %d\tAcc. PWR: %d\t Inst PWR: %d\n", pwr_event_count, pedal_pwr, instant_cadence, accumulated_pwr, instant_pwr);
}

PWRPage50::PWRPage50() :
    hw_revision(0),
    manufacturer_id(0),
    model_number(0)
{}

void PWRPage50::Decode(uint8_t const* buffer)
{
    ant_pwr_page50_data_layout_t const* p_incoming_data = (ant_pwr_page50_data_layout_t *)buffer;
    hw_revision = p_incoming_data->hw_revision;
    manufacturer_id = (uint16_t)((p_incoming_data->manufacturer_id_msb << 8) + p_incoming_data->manufacturer_id_lsb);
    model_number = (uint16_t)((p_incoming_data->model_number_msb << 8) + p_incoming_data->model_number_lsb);
}

void PWRPage50::Encode(uint8_t* buffer)
{
    ant_pwr_page50_data_layout_t * p_outgoing_data = (ant_pwr_page50_data_layout_t *)buffer;
    p_outgoing_data->reserved[0] = UINT8_MAX;
    p_outgoing_data->reserved[1] = UINT8_MAX;
    p_outgoing_data->hw_revision = hw_revision;
    p_outgoing_data->manufacturer_id_lsb = (uint8_t)(manufacturer_id & UINT8_MAX);
    p_outgoing_data->manufacturer_id_msb = (uint8_t)((manufacturer_id >> 8) & UINT8_MAX);
    p_outgoing_data->model_number_lsb = (uint8_t)(model_number & UINT8_MAX);
    p_outgoing_data->model_number_msb = (uint8_t)((model_number >> 8) & UINT8_MAX);
}
PWRPage51::PWRPage51() :
    sw_revision_supplemental(0),
    sw_revision_main(0),
    serial_number(0)
{}

void PWRPage51::Decode(uint8_t const* buffer)
{
    ant_pwr_page51_data_layout_t const* p_incoming_data = (ant_pwr_page51_data_layout_t *)buffer;
    sw_revision_supplemental = p_incoming_data->sw_revision_supplemental;
    sw_revision_main = p_incoming_data->sw_revision_main;
    serial_number = uint32_decode(p_incoming_data->serial_number);
}

void PWRPage51::Encode(uint8_t* buffer)
{
    ant_pwr_page51_data_layout_t * p_outgoing_data = (ant_pwr_page51_data_layout_t *)buffer;
    p_outgoing_data->reserved = UINT8_MAX;
    p_outgoing_data->sw_revision_supplemental = sw_revision_supplemental;
    p_outgoing_data->sw_revision_main = sw_revision_main;
    UNUSED_PARAMETER(uint32_encode(serial_number, p_outgoing_data->serial_number));
}

PWRPage01::PWRPage01() :
    calibration_id(0),
    auto_zero_status(0),
    calibration_data(0)
{}

void PWRPage01::Decode(uint8_t const* buffer)
{
    ant_pwr_page01_data_layout_t const* p_incoming_data = (ant_pwr_page01_data_layout_t *)buffer;
    calibration_id = p_incoming_data->calibration_id;
    auto_zero_status = p_incoming_data->auto_zero_status;
    calibration_data = (uint16_t)((p_incoming_data->calibration_data_msb << 8) + p_incoming_data->calibration_data_lsb);
}

void PWRPage01::Encode(uint8_t* buffer)
{
    ant_pwr_page01_data_layout_t * p_outgoing_data = (ant_pwr_page01_data_layout_t *)buffer;
    p_outgoing_data->calibration_id = calibration_id;
    p_outgoing_data->auto_zero_status = auto_zero_status;
    p_outgoing_data->reserved[0] = UINT8_MAX;
    p_outgoing_data->reserved[1] = UINT8_MAX;
    p_outgoing_data->reserved[2] = UINT8_MAX;
    p_outgoing_data->calibration_data_lsb = (uint8_t)(calibration_data & UINT8_MAX);
    p_outgoing_data->calibration_data_msb = (uint8_t)((calibration_data >> 8) & UINT8_MAX);
}

PWRPage46::PWRPage46() :
    descriptor_byte_1(0),
    descriptor_byte_2(0),
    requested_transmission_response(0),
    requested_page_number(0),
    command_type(0)
{}

void PWRPage46::Decode(uint8_t const* buffer)
{
    ant_pwr_page46_data_layout_t const* p_incoming_data = (ant_pwr_page46_data_layout_t *)buffer;
    descriptor_byte_1 = p_incoming_data->descriptor_byte_1;
    descriptor_byte_2 = p_incoming_data->descriptor_byte_2;
    requested_transmission_response = p_incoming_data->requested_transmission_response;
    requested_page_number = p_incoming_data->requested_page_number;
    command_type = p_incoming_data->command_type;
}

void PWRPage46::Encode(uint8_t* buffer)
{
    ant_pwr_page46_data_layout_t * p_outgoing_data = (ant_pwr_page46_data_layout_t *)buffer;
    p_outgoing_data->descriptor_byte_1 = descriptor_byte_1;
    p_outgoing_data->descriptor_byte_2 = descriptor_byte_2;
    p_outgoing_data->requested_transmission_response = requested_transmission_response;
    p_outgoing_data->requested_page_number = requested_page_number;
    p_outgoing_data->command_type = command_type;
}

PWRPage52::PWRPage52() :
    battery_identifier(0),
    cumulative_operating_time(0),
    fractional_battery_voltage(0),
    descriptive_bitfield(0)
{}

void PWRPage52::Decode(uint8_t const* buffer)
{
    ant_pwr_page52_data_layout_t const* p_incoming_data = (ant_pwr_page52_data_layout_t *)buffer;
    battery_identifier = p_incoming_data->battery_identifier;
    cumulative_operating_time = uint24_decode(p_incoming_data->cumulative_operating_time);
    fractional_battery_voltage = p_incoming_data->fractional_battery_voltage;
    descriptive_bitfield = p_incoming_data->descriptive_bitfield;
}

void PWRPage52::Encode(uint8_t* buffer)
{
    ant_pwr_page52_data_layout_t * p_outgoing_data = (ant_pwr_page52_data_layout_t *)buffer;
    p_outgoing_data->battery_identifier = battery_identifier;
    UNUSED_PARAMETER(uint24_encode(cumulative_operating_time, p_outgoing_data->cumulative_operating_time));
    p_outgoing_data->fractional_battery_voltage = fractional_battery_voltage;
    p_outgoing_data->descriptive_bitfield = descriptive_bitfield;
}

PWRPage56::PWRPage56() :
    peripheral_device_index(0),
    total_number_devices(0),
    channel_state(0),
    peripheral_device_number(0),
    peripheral_device_transmission_type(0),
    peripheral_device_type(0)
{}

void PWRPage56::Decode(uint8_t const* buffer)
{
    ant_pwr_page56_data_layout_t const* p_incoming_data = (ant_pwr_page56_data_layout_t *)buffer;
    peripheral_device_index = p_incoming_data->peripheral_device_index;
    total_number_devices = p_incoming_data->total_number_devices;
    channel_state = p_incoming_data->channel_state;
    peripheral_device_number = (uint16_t)((p_incoming_data->peripheral_device_number_msb << 8) + p_incoming_data->peripheral_device_number_lsb);
    peripheral_device_transmission_type = p_incoming_data->peripheral_device_transmission_type;
    peripheral_device_type = p_incoming_data->peripheral_device_type;
}

void PWRPage56::Encode(uint8_t* buffer)
{
    ant_pwr_page56_data_layout_t * p_outgoing_data = (ant_pwr_page56_data_layout_t *)buffer;
    p_outgoing_data->peripheral_device_index = peripheral_device_index;
    p_outgoing_data->total_number_devices = total_number_devices;
    p_outgoing_data->channel_state = channel_state;
    p_outgoing_data->peripheral_device_number_lsb = (uint8_t)(peripheral_device_number & UINT8_MAX);
    p_outgoing_data->peripheral_device_number_msb = (uint8_t)((peripheral_device_number >> 8) & UINT8_MAX);
    p_outgoing_data->peripheral_device_transmission_type = peripheral_device_transmission_type;
    p_outgoing_data->peripheral_device_type = peripheral_device_type;
}

PWRPage02::PWRPage02() :
    subpage_number(0)
{}

void PWRPage02::Decode(uint8_t const* buffer)
{
    ant_pwr_page02_data_layout_t const* p_incoming_data = (ant_pwr_page02_data_layout_t *)buffer;
    subpage_number = p_incoming_data->subpage_number;
    subpage_data[0] = p_incoming_data->subpage_data[0];
    subpage_data[1] = p_incoming_data->subpage_data[1];
    subpage_data[2] = p_incoming_data->subpage_data[2];
    subpage_data[3] = p_incoming_data->subpage_data[3];
    subpage_data[4] = p_incoming_data->subpage_data[4];
    subpage_data[5] = p_incoming_data->subpage_data[5];
}

void PWRPage02::Encode(uint8_t* buffer)
{
    ant_pwr_page02_data_layout_t * p_outgoing_data = (ant_pwr_page02_data_layout_t *)buffer;
    p_outgoing_data->subpage_number = subpage_number;
    p_outgoing_data->subpage_data[0] = subpage_data[0];
    p_outgoing_data->subpage_data[1] = subpage_data[1];
    p_outgoing_data->subpage_data[2] = subpage_data[2];
    p_outgoing_data->subpage_data[3] = subpage_data[3];
    p_outgoing_data->subpage_data[4] = subpage_data[4];
    p_outgoing_data->subpage_data[5] = subpage_data[5];
}

BicyclePower::BicyclePower(ANTTransmissionMode mode) :
    ANTProfile(mode)
    {
        m_disp_config.channel_type      = PWR_DISP_CHANNEL_TYPE;
        m_disp_config.ext_assign        = PWR_EXT_ASSIGN;
        m_disp_config.rf_freq           = PWR_RF_CHANNEL;
        m_disp_config.transmission_type = 0;
        m_disp_config.device_type       = PWR_DEVICE_TYPE;
        m_disp_config.channel_period    = PWR_MSG_PERIOD_4Hz;
        m_disp_config.network_number    = ANTPLUS_NETWORK_NUMBER;
        m_disp_config.device_number     = PWR_DEVICE_NUMBER;


        m_channel_sens_config.channel_type       = PWR_SENS_CHANNEL_TYPE;
        m_channel_sens_config.ext_assign         = PWR_EXT_ASSIGN;
        m_channel_sens_config.rf_freq            = PWR_RF_CHANNEL;
        m_channel_sens_config.transmission_type  = 1;
        m_channel_sens_config.device_type        = PWR_DEVICE_TYPE;
        m_channel_sens_config.channel_period     = PWR_MSG_PERIOD_4Hz;
        m_channel_sens_config.device_number      = PWR_DEVICE_NUMBER;
        m_channel_sens_config.network_number     = ANTPLUS_NETWORK_NUMBER;

        main_page_number = ANT_PWR_PAGE_10;
        bkgd_page0_number = ANT_PWR_PAGE_50;
        bkgd_page1_number = ANT_PWR_PAGE_51;
        bkgd_page2_number = ANT_PWR_PAGE_52;
        bkgd_page3_number = ANT_PWR_PAGE_56;
        calibration_page_number = ANT_PWR_PAGE_01;
        message_counter = 0;
        
        // page_52_present = false;
        // ext_page_number = ANT_PWR_PAGE_52;
        page10.SetPedalPWR(0xFFu);              //0xff for OFF
        page10.SetInstantCadence(0xFFu);        //0xff for OFF
        page50.SetHwRevision(0x01u);            //v1
        page50.SetManufacturerID(0x000Fu);      //15 for dynastream
        page50.SetModelNumber(0x1B39);          //6969 for fun
        page51.SetSWRevisionSuppl(0xFFu);       //OFF
        page51.SetSWRevisionMain(0x01u);        //v1
        page51.SetSerialNumber(0x00B8AAF6u);    //12102390

    }


BicyclePower::ant_pwr_page_t BicyclePower::GetNextPageNumber()
{
    ant_pwr_page_t page_number;
    if (need_answer)
    {
        need_answer = false;
        switch (requested_page)
        {
        case 0x50:
            page_number = bkgd_page0_number;
            non_main_messages++;
            break;
        case 0x51:
            page_number = bkgd_page1_number;
            non_main_messages++;
            break;
        case 0x52:
            page_number = bkgd_page2_number;
            non_main_messages++;
            break;
        case 0x56:
            page_number = bkgd_page3_number;
            non_main_messages++;
            break;
        case 0x02:
            page_number = ANT_PWR_PAGE_02;
            non_main_messages++;
            break;
        default:
            break;
        }
        if (page_number == ANT_PWR_PAGE_02)
        {
            switch (requested_subpage)
            {
            case 0x01:
                page02.SetSubPageNumber(0x01u);
                page02.SetSubpageData(0, 0xFFu);        //Byte 2 of answer
                page02.SetSubpageData(1, 0xFFu);        //Byte 3 of answer
                page02.SetSubpageData(2, 0x7Du);        //172.5mm Crank length
                page02.SetSubpageData(3, 0b00000011);   //No Custom Cal. Not two individual Sensors Bit 2-5. Crank Length Fixed
                page02.SetSubpageData(4, 0x00);
                page02.SetSubpageData(5, 0xFFu);
                break;
            case 0xFD:
                page02.SetSubPageNumber(0xFDu);
                page02.SetSubpageData(0, 0b11111110);   //Byte 2 of answer
                page02.SetSubpageData(1, 0x45u);        //Byte 3 of answer
                page02.SetSubpageData(2, 0b11111110);   //
                page02.SetSubpageData(3, 0x45u);        //0b00000011
                page02.SetSubpageData(4, 0b11111110);   //
                page02.SetSubpageData(5, 0x45u);        //
                break;
            case 0xFE:
                page02.SetSubPageNumber(0xFEu);
                page02.SetSubpageData(0, 0xFFu);        //Byte 2 of answer reserved
                page02.SetSubpageData(1, 0xFFu);        //Byte 3 of answer reserved
                page02.SetSubpageData(2, 0b11111110);   //
                page02.SetSubpageData(3, 0xFFu);        //0b00000011
                page02.SetSubpageData(4, 0b11111110);   //
                page02.SetSubpageData(5, 0xFFu);        //
                break;
            
            default:
                break;
            }
            Serial.printf("Set Subpage Values acc: 0x%.2X\n", requested_subpage);
        }
        //Serial.printf("requested_page: 0x%.2X\n", requested_page);
    }
    else if (calibration_request && non_main_messages < 2) //If calibration request is there next page is Calibration Page but need to set params
    {
        page_number = calibration_page_number;
        calibration_request = false;
        non_main_messages++;
        page01.SetCalibrationID(0xACu);
        page01.SetAutoZeroStatus(0x00u);
        page01.SetCalibrationData(0x0000u);
        if (message_counter == (BACKGROUND_DATA_INTERVAL)) {
            background_page_offset++;
        }
    }
    else if (message_counter == (BACKGROUND_DATA_INTERVAL + background_page_offset) && non_main_messages < 2)
    {
        page_number = bkgd_page0_number;
        page50.SetHwRevision(0x01u);            //v1
        page50.SetManufacturerID(0x000Fu);      //15 for dynastream
        page50.SetModelNumber(0x1B39);          //6969 for fun

        non_main_messages++;
    } 
    else if (message_counter > (BACKGROUND_DATA_INTERVAL + background_page_offset) && non_main_messages < 2)
    {
        page_number = bkgd_page1_number;
        page51.SetSWRevisionSuppl(0xFFu);       //OFF
        page51.SetSWRevisionMain(0x01u);        //v1
        page51.SetSerialNumber(0x00B8AAF6u);    //12102390

        message_counter = 0;
        background_page_offset = 0;
        non_main_messages++;
    }
    else
    {
        page_number = main_page_number;
        non_main_messages = 0;
    }
    //Serial.printf("page_number: %d\t\tmessage_counter: %d\n", page_number, message_counter);
    message_counter++;
    return page_number;
}

void BicyclePower::EncodeMessage()
{
    ant_pwr_message_layout_t * p_pwr_message_payload = (ant_pwr_message_layout_t *)m_message_payload;
    p_pwr_message_payload->page_number = GetNextPageNumber();
    switch (p_pwr_message_payload->page_number)
    {
        case ANT_PWR_PAGE_10: //Main Page
            page10.Encode(p_pwr_message_payload->page_payload);
            //Serial.printf("Encoding Page 0x10\n");
            break;
        case ANT_PWR_PAGE_50: //Manufacturer Info
            page50.Encode(p_pwr_message_payload->page_payload);
            //Serial.printf("Encoding Page 0x50\n");
            break;
        case ANT_PWR_PAGE_51: //Product Info
            page51.Encode(p_pwr_message_payload->page_payload);
            //Serial.printf("Encoding Page 0x51\n");
            break;
        case ANT_PWR_PAGE_01: //Calibration Page
            page01.Encode(p_pwr_message_payload->page_payload);
            //Serial.printf("Encoding Page 0x01\n");
            break;
        case ANT_PWR_PAGE_52: //Battery Page
            page52.Encode(p_pwr_message_payload->page_payload);
            //Serial.printf("Encoding Page 0x52\n");
            break;
        case ANT_PWR_PAGE_56: //Paired Devices Page
            page56.Encode(p_pwr_message_payload->page_payload);
            //Serial.printf("Encoding Page 0x56\n");
            break;
        case ANT_PWR_PAGE_46: //Request Data Page
            page46.Encode(p_pwr_message_payload->page_payload);
            //Serial.printf("Encoding Page 0x46\n");
            break;
        case ANT_PWR_PAGE_02: //Get Set Parameters page
            page02.Encode(p_pwr_message_payload->page_payload);
            // Serial.printf("Encoding Page 0x02\n");
            break;
        default:
            break;
    }
}

void BicyclePower::DecodeMessage(uint8_t* buffer)
{
    ant_pwr_message_layout_t * p_pwr_message_payload = (ant_pwr_message_layout_t *)buffer;
    Serial.printf("0x%.2X\t", p_pwr_message_payload->page_number);
    for (int i = 0; i < 7; ++i)
    {
        Serial.printf("%.2X ", p_pwr_message_payload->page_payload[i]);
    }
    Serial.printf("\n");
    //Serial.printf("\t%d\n", p_pwr_message_payload->page_payload[5]);
    switch (p_pwr_message_payload->page_number)
    {
        case ANT_PWR_PAGE_10: //Main Page
            page10.Decode(p_pwr_message_payload->page_payload);
            Serial.printf("\tDecoding Page 0x10\n");
            break;
        case ANT_PWR_PAGE_50: //Manufacturer Info
            page50.Decode(p_pwr_message_payload->page_payload);
            Serial.printf("\tDecoding Page 0x50\n");
            break;
        case ANT_PWR_PAGE_51: //Product Info
            page51.Decode(p_pwr_message_payload->page_payload);
            Serial.printf("\tDecoding Page 0x51\n");
            break;
        case ANT_PWR_PAGE_01: //Calibration Page
            page01.Decode(p_pwr_message_payload->page_payload);
            Serial.printf("\tDecoding Page 0x01\n");
            cal_id = page01.GetCalibrationID();
            if (cal_id == 0xAA || cal_id == 0xAB) //In case we need a calibration response
            {
                calibration_request = true;
            }
            break;
        case ANT_PWR_PAGE_52: //Battery Page
            page52.Decode(p_pwr_message_payload->page_payload);
            Serial.printf("\tDecoding Page 0x52\n");
            break;
        case ANT_PWR_PAGE_56: //Paired Devices Page
            page56.Decode(p_pwr_message_payload->page_payload);
            Serial.printf("\tDecoding Page 0x56\n");
            break;
        case 0x46: //Request Data Page
            page46.Decode(p_pwr_message_payload->page_payload);
            requested_page = page46.GetRequestedPageNumber();
            requested_subpage = page46.GetDescriptorByte1();
            need_answer = true;
            Serial.printf("\tDecoding Page 0x46 -> Wanted: 0x%.2X Sub: 0x%.2X\n", requested_page, requested_subpage);
            break;
        case ANT_PWR_PAGE_02: //Get Set Parameters page
            page02.Decode(p_pwr_message_payload->page_payload);
            Serial.printf("\tDecoding Page 0x02\n");
            break;
        default:
            
            break;
    }
}