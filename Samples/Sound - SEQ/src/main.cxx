#include "SDDRVS.DAT"
#define SoundMem 0x25a0b000
#include <srl.hpp>

// SEQ sample (Sega MIDI-style music playback) for SRL
// Heavely inspired by the Jo Engine sample made by jfsantos (João Felipe Santos)
// https://github.com/jfsantos/mid2seq

uint8_t mechs_map[] = {
    0x30,0x00,0xC0,0x00,0x80,0x01,0x00,0x40,0x20,0x01,0xC0,0x40,0x80,0x00,
    0x05,0x40,0x10,0x02,0x45,0x80,0x80,0x00,0x01,0x6E,0x00,0x02,0x46,0xEE,
    0x80,0x00,0x88,0xEC,0x11,0x02,0xCF,0xDC,0x80,0x00,0x53,0xA0,0x01,0x03,
    0x23,0x7C,0x80,0x03,0x34,0x84,0x12,0x06,0x55,0x54,0x80,0x00,0x28,0x5A,
    0x02,0x06,0x7D,0xAE,0x80,0x01,0x82,0x52,0xFF,0xFF
};

void Initialize_SoundSystem_SEQ_SaturnMIDI(void)
{
    slInitSound(sddrvstsk, sizeof(sddrvstsk), (uint8_t *)mechs_map, sizeof(mechs_map) );

    SRL::Cd::File seqFILE("BGM01.SEQ");
    SRL::Cd::File tonFILE("BGM01.TON");

    uint8_t* seqBuf = new uint8_t[seqFILE.Size.Bytes];
    uint8_t* tonBuf = new uint8_t[tonFILE.Size.Bytes];

    seqFILE.LoadBytes(0, seqFILE.Size.Bytes, seqBuf);
    tonFILE.LoadBytes(0, tonFILE.Size.Bytes, tonBuf);

    // SEQ data
    slDMACopy(seqBuf, (void *)((uint8_t*)SoundMem + 0x21fdc), seqFILE.Size.Bytes);

    // TON data
    slDMACopy(tonBuf, (void *)((uint8_t*)SoundMem + 0x2737c), tonFILE.Size.Bytes);

    // Clear
    delete[] seqBuf;
    delete[] tonBuf;
    seqFILE.Close();
    tonFILE.Close();
}

int main()
{
    SRL::Core::Initialize(SRL::Types::HighColor::Colors::Black);

    // Load and initialize a Saturn SEQ music track (Sega MIDI-style music playback)
    Initialize_SoundSystem_SEQ_SaturnMIDI();

    // Initialize gamepad
    SRL::Input::Digital pad(0);

    bool seqPlay = true;

    // Volume (0 = 0% | 127 = 100%)
    uint8_t seqVolume = 127;

    // Auto-Play the SEQ music track
    slBGMOn((1 << 8) + 0, 0, seqVolume, 0);

    // Tempo (a value ranging from -32768 to 32767 for the parameter "Tempo")
    int16_t musicTempo = 0;
    slBGMTempo(musicTempo);

    while (true)
    {
        SRL::Debug::Print(1,1, "Sound SEQ sample");

        SRL::Debug::Print(1,5, "Press Up/Down to change the Volume");
        if (pad.WasPressed(SRL::Input::Digital::Button::Up))
        {
            if (seqVolume < 127)
            {
                seqVolume++;
                slBGMFade(seqVolume, 0);
            }
        }
        else if (pad.WasPressed(SRL::Input::Digital::Button::Down))
        {
            if (seqVolume > 0)
            {
                seqVolume--;
                slBGMFade(seqVolume, 0);
            }
        }
        SRL::Debug::Print(3,6, "Current Volume:    ");
        SRL::Debug::Print(3+16,6, "%d", seqVolume);

        SRL::Debug::Print(1,9, "Press Left/Right to change the Tempo");
        if (pad.WasPressed(SRL::Input::Digital::Button::Left))
        {
            if (musicTempo > -32768+16)
            {
                musicTempo -= 16;
                slBGMTempo(musicTempo);
            }
        }
        else if (pad.WasPressed(SRL::Input::Digital::Button::Right))
        {
            if (musicTempo < 32767-16)
            {
                musicTempo += 16;
                slBGMTempo(musicTempo);
            }
        }
        SRL::Debug::Print(3,10, "Current Tempo:        ");
        SRL::Debug::Print(3+15,10, "%d", musicTempo);

        SRL::Debug::Print(1,13, "Press START to Stop/Start the Music");
        if (pad.WasPressed(SRL::Input::Digital::Button::START))
        {
            if (seqPlay)
            {
                seqPlay = false;
                slBGMOff();
            }
            else
            {
                seqPlay = true;
                slBGMOn((1 << 8) + 0, 0, seqVolume, 0);
                slBGMTempo(musicTempo);
            }
        }

        if (seqPlay)
        {
            SRL::Debug::Print(3,14, "SEQ Music Playing...");
        }
        else
        {
            SRL::Debug::Print(3,14, "No SEQ Music...     ");
        }

        SRL::Core::Synchronize();
    }

    return 0;
}
