#define SRL_ENABLE_AUDIO_SEQ_SUPPORT 1
#define SoundMem 0x25a0b000 // The playback data is stored starting from address 0xB000 in the MC68000 memory area (0x25A0B000 in the program) https://antime.kapsi.fi/sega/files/ST-237-R1-051795.pdf
#include <srl.hpp>

// SEQ sample (Sega MIDI-style music playback) for SRL
// Heavely inspired by the Jo Engine sample made by jfsantos (João Felipe Santos)
// https://github.com/jfsantos/mid2seq

void Load_SEQ_SaturnMIDI(void)
{
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
    Load_SEQ_SaturnMIDI();

    // Initialize gamepad
    SRL::Input::Digital pad(0);

    // Load PCM sound to test if PCM could be played while SEQ music is also played
    SRL::Cd::File file("GUN.PCM");
    SRL::Sound::Pcm::RawPcm gun(&file, SRL::Sound::Pcm::PcmChannels::Mono, SRL::Sound::Pcm::Pcm8Bit, 15360);

    bool seqPlay = true;

    uint16_t seqSongIndex = (1 << 8) + 0; // + 0 <<< For 1st Song Index

    uint8_t seqPriority = 0; // Priority (range only from 0 to 31. The larger the value, the higher the priority)

    uint8_t seqVolume = 127; // Volume (range only from 0 to 127. 0 = 0% | 127 = 100%)

    uint8_t seqRate = 0; // Rate (range from 0 to 255)

    // slBGMOn(SongIndex, Priority, Volume, Rate) | Auto-Play the SEQ music track
    slBGMOn(seqSongIndex, seqPriority, seqVolume, seqRate);

    // Tempo (a value ranging from -32768 to 32767 for the parameter Tempo. 0 = Normal Tempo)
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
            if (musicTempo > INT16_MIN+16)
            {
                musicTempo -= 16;
                slBGMTempo(musicTempo);
            }
        }
        else if (pad.WasPressed(SRL::Input::Digital::Button::Right))
        {
            if (musicTempo < INT16_MAX-16)
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


        SRL::Debug::Print(1,17, "Press A to Play PCM Gun Shot");
        if (pad.WasPressed(SRL::Input::Digital::Button::A))
        {
            gun.PlayOnChannel(0);
        }

        SRL::Core::Synchronize();
    }

    return 0;
}
