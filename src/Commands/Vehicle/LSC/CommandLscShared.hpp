#pragma once

#include <atomic>

namespace Stand
{
    namespace
    {
        static std::atomic<bool> s_lsc_in_veh{false};
        static std::atomic<bool> s_lsc_show_na{false};

        namespace Mod
        {
            constexpr int Spoilers    = 0;
            constexpr int FrontBumper = 1;
            constexpr int RearBumper  = 2;
            constexpr int SideSkirt   = 3;
            constexpr int Exhaust     = 4;
            constexpr int Frame       = 5;
            constexpr int Grille      = 6;
            constexpr int Hood        = 7;
            constexpr int Fender      = 8;
            constexpr int RFender     = 9;
            constexpr int Roof        = 10;
            constexpr int Engine      = 11;
            constexpr int Brakes      = 12;
            constexpr int Trans       = 13;
            constexpr int Horns       = 14;
            constexpr int Suspension  = 15;
            constexpr int Armour      = 16;
            constexpr int Turbo       = 18;
            constexpr int FrontWheels = 23;
            constexpr int PlateHolder = 25;
            constexpr int VanityPlates= 26;
            constexpr int Trim        = 27;
            constexpr int Ornaments   = 28;
            constexpr int Dashboard   = 29;
            constexpr int Dial        = 30;
            constexpr int DoorSpeaker = 31;
            constexpr int Seats       = 32;
            constexpr int Wheel       = 33;
            constexpr int Shifter     = 34;
            constexpr int Plaques     = 35;
            constexpr int Speakers    = 36;
            constexpr int Trunk       = 37;
            constexpr int Hydraulics  = 38;
            constexpr int EngineBlock = 39;
            constexpr int AirFilter   = 40;
            constexpr int Struts      = 41;
            constexpr int ArchCover   = 42;
            constexpr int Aerials     = 43;
            constexpr int Trim2       = 44;
            constexpr int Tank        = 45;
            constexpr int DoorL       = 46;
            constexpr int DoorR       = 47;
            constexpr int Livery      = 48;
            constexpr int Lightbar    = 49;

            static constexpr int visual[] = {
                Horns, SideSkirt, Suspension, Exhaust,
                FrontBumper, RearBumper, Hood, Roof, Fender, RFender,
                Grille, PlateHolder, VanityPlates, Trim, Ornaments,
                Dashboard, Dial, Seats, DoorSpeaker, Wheel, Shifter,
                Plaques, Speakers, Trunk, Hydraulics, EngineBlock,
                AirFilter, Struts, ArchCover, Aerials, Trim2, Tank,
                DoorL, DoorR, Lightbar, Livery, Frame,
            };

            const char* GetName(int t)
            {
                switch (t) {
                case Spoilers:    return "Spoilers";
                case FrontBumper: return "Front Bumper";
                case RearBumper:  return "Rear Bumper";
                case SideSkirt:   return "Side Skirt";
                case Exhaust:     return "Exhaust";
                case Frame:       return "Frame";
                case Grille:      return "Grille";
                case Hood:        return "Hood";
                case Fender:      return "Fender";
                case RFender:     return "Right Fender";
                case Roof:        return "Roof / Weapons";
                case Engine:      return "Engine";
                case Brakes:      return "Brakes";
                case Trans:       return "Transmission";
                case Horns:       return "Horns";
                case Suspension:  return "Suspension";
                case Armour:      return "Armour";
                case Turbo:       return "Turbo";
                case FrontWheels: return "Front Wheels";
                case PlateHolder: return "Plate Holders";
                case VanityPlates:return "Vanity Plates";
                case Trim:        return "Trim Design";
                case Ornaments:   return "Ornaments";
                case Dashboard:   return "Dashboard";
                case Dial:        return "Dial Design";
                case DoorSpeaker: return "Door Speaker";
                case Seats:       return "Seats";
                case Wheel:       return "Steering Wheel";
                case Shifter:     return "Shift Lever";
                case Plaques:     return "Plaques";
                case Speakers:    return "Speakers";
                case Trunk:       return "Trunk";
                case Hydraulics:  return "Hydraulics";
                case EngineBlock: return "Engine Block";
                case AirFilter:   return "Boost / Air Filter";
                case Struts:      return "Struts";
                case ArchCover:   return "Arch Cover";
                case Aerials:     return "Aerials";
                case Trim2:       return "Trim";
                case Tank:        return "Tank";
                case DoorL:       return "Left Door";
                case DoorR:       return "Right Door";
                case Livery:      return "Livery";
                case Lightbar:    return "Lightbar";
                default:          return "Mod";
                }
            }
        }
    }
}
