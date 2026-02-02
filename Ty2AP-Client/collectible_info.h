#pragma once

struct LevelCollectibleInfo {
    std::string levelName;
    std::vector<std::pair<std::string, std::string>> cogs;
    std::vector<std::pair<std::string, std::string>> bilbies;
    std::vector<std::pair<std::string, std::string>> orbs;
    std::vector<std::pair<std::string, std::string>> pictureFrames;
    std::vector<std::pair<std::string, std::string>> steves;
    std::vector<std::pair<std::string, std::string>> frills;

    LevelCollectibleInfo(
        const std::string& levelName,
        const std::vector<std::pair<std::string, std::string>>& cogs,
        const std::vector<std::pair<std::string, std::string>>& bilbies,
        const std::vector<std::pair<std::string, std::string>>& orbs,
        const std::vector<std::pair<std::string, std::string>>& pictureFrames,
        const std::vector<std::pair<std::string, std::string>>& steves,
        const std::vector<std::pair<std::string, std::string>>& frills
    )
        : levelName(levelName),
        cogs(cogs),
        bilbies(bilbies),
        orbs(orbs),
        pictureFrames(pictureFrames),
        steves(steves),
        frills(frills)
    {
    }
};

const LevelCollectibleInfo BuramudgeeInfo(
    "Burramudgee",
    {
        // Cogs
        {"Cog 1", "Spy Eggs near river"},
        {"Cog 2", "Grind rail in upper section of town"},
        {"Cog 3", "Top of Spooky Mansion"},
        {"Cog 4", "Floating platform above HQ"},
    },
    {
        // Bilbies
        {"Bilby 1", "Rock beneath HQ ramp"},
        {"Bilby 2", "Platform on back of cliff under house above HQ"},
        {"Bilby 3", "Roof of HQ"},
        {"Bilby 4", "Top of lookout tower by HQ"},
    },
    {
        // Orbs
        {"Orb 1", "Lash to floating platform in town"},
        {"Orb 2", "Fan platform above the river"},
        {"Orb 3", "Ledge on upper section of town"},
        {"Orb 4", "Platform in pond at HQ"},
    },
    {
        // Picture Frames
        {"Frames 1-3", "Fab platform above HQ"},
        {"Frames 4-7", "HQ watch tower"},
        {"Frames 8-31", "Burramudgee Sewers"},
        {"Frames 32-46", "HQ balcony crates"},
        {"Frames 47-49", "HQ warp crystal smash crates"},
        {"Frames 50-52", "Left ledge smash crates in hangar (looking from entrance)"},
        {"Frames 53-57", "Roof smash crate"},
        {"Frames 58-62", "Meeting room smash crate"},
        {"Frames 63-67", "Right ledge hangar smash crates"},
    },
    {
        // Steves
        {"Steve", "Sewers"},
    },
    {
        // Frills
        {"Frill 1", "Beneath Sgt. Bluey"},
        {"Frill 2", "Beside stairs to sewers"},
    }
);

const LevelCollectibleInfo OOInfo(
    "Outback Oasis",
    {
        // Cogs
        {"Cog 1", "Rock wall by dunny behind smash rock wall right of level entrance"},
        {"Cog 2", "Behind wall by Frill 2"},
        {"Cog 3", "Next to Bilby 1"},
        {"Cog 4", "Fan platform parkour outside cave entrance"},
    },
    {
        // Bilbies
        {"Bilby 1", "Hanging from tree at end of path behind smash rock right of level entrance"},
        {"Bilby 2", "Hanging above level entrance"},
        {"Bilby 3", "Fan platforms above sleeping uber frill cave"},
    },
    {
        // Orbs
        {"Orb 1", "Beside bunyip behind smash rock wall right of level entrance - Defeat Uber Frills"},
        {"Orb 2", "Center of spinning platforms in lake area"},
    },
    {
        // Picture Frames
        {"Picture Frames 1-3", "Warp Crystal"},
        {"Picture Frames 4-13", "Invis crates in sleeping uber frill cave"},
    },
    {
        // Steves
        {"Steve", "Lake area - Hit button in back of cave to open door"},
    },
    {
        // Frills
        {"Frill 1", "Beside Bunyip behind smash rock wall right of level entrance"},
        {"Frill 2", "Past warp crystal"},
        {"Frill 3", "Ledge overlooking sleeping uber frill cave"},
        {"Frill 4", "Picnic outside level entrance"},
    }
);

const LevelCollectibleInfo SulphurRocksInfo(
    "Sulphur Rocks",
    {
        // Cogs
        {"Cog 1", "Lava area in Princess Orchid's Trial"},
        {"Cog 2", "Lifter Bunyip rock"},
        {"Cog 3", "Lash around corner near start"},
        {"Cog 4", "Pillars before Beetle Chase mission"},
        {"Cog 5", "Central pillar near Steve Gate"},
        {"Cog 6", "Side ledge near Steve Gate"},
        {"Cog 7", "Behind saw door after ladders near start"},
        {"Cog 8", "South of shipping containers near start"},
        {"Cog 9", "Bench at warp crystal by north fork to lava section"},
    },
    {
        // Bilbies
        {"Bilby 1", "Cliff to left after orb 5"},
        {"Bilby 2", "Between metal walls after ladders near start"},
        {"Bilby 3", "Fan platforms after ladders near start"},
        {"Bilby 4", "North of shipping containers near start"},
        {"Bilby 5", "Platform beneath orb 7"},
    },
    {
        // Orbs
        {"Orb 1", "West ledge above river"},
        {"Orb 2", "Lash parkour in lava section"},
        {"Orb 3", "Skull pressure plate pillars south of river"},
        {"Orb 4", "Disappearing fan platforms outside level entrance"},
        {"Orb 5", "Platform with uber frill"},
        {"Orb 6", "Ledge above fire-breathing frills in Princess Orchid's Trial"},
        {"Orb 7", "Invisible platforms above river"},
    },
    {
        // Picture Frames
        {"Frames 1-2", "Surrounding cog behind shipping container at start"},
        {"Frame 3", "In canyon to the left before Princess Orchid"},
        {"Frames 4-6", "Fence before Princess Orchid with koalas"},
        {"Frame 7", "In canyon to the left before Princess Orchid"},
        {"Frames 8-13", "Invis crates in canyon at start"},
        {"Frames 14-17", "Invis crates above shipping container at start"},
        {"Frames 18-21", "Invis crates above Princess Orchid"},
        {"Frame 22", "Surrounding cog behind shipping container at start"},
    },
    {
        // Steves
        {"Steve", "Large pillar surrounded by crocs - hit button to open gate"},
    },
    {
        // Frills
        {"Frill 1", "Cliff at start"},
        {"Frill 2", "In canyon south of river"},
        {"Frill 3", "Behind wall before Princess Orchid"},
    }
);

const LevelCollectibleInfo MBInfo(
    "Mount Boom",
    {
        // Cogs
        {"Cog 1", "Warp crystal behind 1st door on right in lava tube"},
        {"Cog 2", "2nd right door in lava tube"},
        {"Cog 3", "Lasharang behind warp crystal behind 2nd right door in lava tube"},
    },
    {
        // Bilbies
        {"Bilby 1", "Hanging on a wall in start section"},
        {"Bilby 2", "Hanging from ladder behind warp crystal behind 2nd right door in lava tube"},
    },
    {
        // Orbs
        {"Orb 1", "Left door in lava tube"},
        {"Orb 2", "Lava door that requires turbines behind warp crystal behind 2nd right door in lava tube"},
    },
    {
        // Picture Frames
        {"Picture Frame", "End of level"},
    },
    {
        // Steves
        {"Steve", "Left door in lava tube"},
    },
    {
        // Frills
        {"Frill 1", "Start of level"},
        {"Frill 2", "End of level"},
    }
);

const LevelCollectibleInfo NNInfo(
    "Never Never",
    {
        // Cogs
        {"Cog 1", "Fan platform at start"},
        {"Cog 2", "Fan platforms behind smash rock past singing dinosaur"},
        {"Cog 3", "Fan platform above Orb 1"},
        {"Cog 4", "Tree below Cog 2"},
        {"Cog 5", "Fan platforms - backtrack from button by Frill 1"},
        {"Cog 6", "Fan platforms in southwest water section"},
        {"Cog 7", "Ladder in tree in southwest water section"},
        {"Cog 8", "Right side ledge in lava section"},
        {"Cog 9", "Flower by thermo bunyip to spy eggs"},
        {"Cog 10", "Left section of water segment to right of lava area on map"},
        {"Cog 11", "First ledge on grind rail after dinosaur section"},
        {"Cog 12", "Timed rail grind at start of dinosaur section"},
        {"Cog 13", "Fan platform above river in dinosaur section - backtrack from button on house"},
        {"Cog 14", "Tree by house at end of level"},
        {"Cog 15", "Fan platforms by Cog 16"},
        {"Cog 16", "Smash rock in lower part of west branch on map"},
    },
    {
        // Bilbies
        {"Bilby 1", "Top right corner of southwest water section"},
        {"Bilby 2", "Left side ledge in lava section"},
        {"Bilby 3", "2nd ledge on grind rail after dinosaur section"},
        {"Bilby 4", "Ledge above Cog 16"},
    },
    {
        // Orbs
        {"Orb 1", "Smash wall before Frill 1"},
        {"Orb 2", "Swing to platform in southwest water section"},
        {"Orb 3", "Lash cliff above button by Frill 1"},
        {"Orb 4", "Behind alien frills past lava section"},
        {"Orb 5", "Final ledge on grind rail after dinosaur section"},
        {"Orb 6", "Swing under land bridge at end of level"},
        {"Orb 7", "Behind water wheel at end of level"},
    },
    {
        // Picture Frames
        {"Frame 1", "Stone platform by trees after first saw"},
        {"Frames 2-3", "Platforms on trees after first saw"},
        {"Frame 4", "Corner of lava section"},
        {"Frame 5", "Behind crates in alien water section"},
        {"Frame 6", "Underwater by alien"},
        {"Frame 7", "In corner next to alien at water section"},
        {"Frame 8", "Underwater by alien"},
        {"Frame 9", "3rd ledge on grind rail after dinosaur section"},
        {"Frame 10", "Smash rock under start of grind rail after dinosaur section"},
        {"Frame 11", "Swing under land bridge"},
        {"Frames 12-15", "Above koalas to the left at end of dino section"},
        {"Frames 16-20", "Invis crate at start"},
        {"Frames 21-25", "Invis crate above alien gathering after singing dinosaur"},
        {"Frames 16-20", "Invis crate by water wheel"},
    },
    {
        // Steves
        {"Steve", "Lava section - Submerge and go through tunnel"},
    },
    {
        // Frills
        {"Frill 1", "By button in section west of lava on map"},
        {"Frill 2", "By Cog 10"},
        {"Frill 3", "By vines at end of dinosaur section"},
        {"Frill 4", "Never Never entrance"},
    }
);

const LevelCollectibleInfo WetlandsInfo(
    "Wetlands",
    {
        // Cogs
        {"Cog 1", "Floating fan platform at start"},
        {"Cog 2", "Behind smash rock at start"},
        {"Cog 3", "Timed button after warp crystal"},
        {"Cog 4", "Spy eggs after warp crystal"},
        {"Cog 5", "Defeat 3 uber frills in bunyip section"},
        {"Cog 6", "Crocodile pen"},
    },
    {
        // Bilbies
        {"Bilby 1", "Inside log at start"},
        {"Bilby 2", "Web section after warp crystal"},
        {"Bilby 3", "Webbed up in bunyip section"},
    },
    {
        // Orbs
        {"Orb 1", "Platform overlooking crocodile pen"},
    },
    {
        // Picture Frames
        {"Frames 1-5", "Warp tree vines"},
        {"Frames 6-11", "Invis crates at start"},
        {"Frames 12-13", "Invis crate at end"},
    },
    {
        // Steves
        {"Steve", "Behind spiders after sawblades"},
    },
    {
        // Frills
        {"Frill 1", "Left of end of fighter bunyip section"},
        {"Frill 2", "By button at start"},
        {"Frill 3", "Koala crew by crocodile pen"},
    }
);

const LevelCollectibleInfo FDInfo(
    "Faire Dinkum",
    {
        // Cogs
        {"Cog 1", "Fan platform at end of level"},
        {"Cog 2", "Smash wall after village gate"},
        {"Cog 3", "Tree platform in south creek section"},
    },
    {
        // Bilbies
        {"Bilby 1", "Tree platform before Frill"},
        {"Bilby 2", "Cliff at end of level"},
        {"Bilby 3", "Spiderwebs in southeast corner"},
        {"Bilby 4", "Pillar before Faire Dinkum entrance"},
    },
    {
        // Orbs
        {"Orb 1", "Tree platforming after village gate"},
        {"Orb 2", "Just after Bilby 3"},
    },
    {
        // Picture Frames
        {"Frame 1", "Tree at start"},
        {"Frames 2-6", "House roofs"},
        {"Frame 7", "Behind animatronic croc in end area"},
        {"Frames 8-9", "Invis crate at start"},
        {"Frames 10-11", "Invis crate in town"},
        {"Frames 12-14", "Invis crates at end"},
    },
    {
        // Steves
    },
    {
        // Frills
        {"Frill", "Beneath walkway after village gate"},
    }
);

const LevelCollectibleInfo FNFInfo(
    "Frill Neck Forest",
    {
        // Cogs
        {"Cog", "Grind rail behind starting tree"},
    },
    {
        // Bilbies
        {"Bilby 1", "End tree platform"},
        {"Bilby 2", "Platforms behind starting tree"},
    },
    {
        // Orbs
        {"Orb", "Platform in middle section"},
    },
    {
        // Picture Frames
        {"Frames 1-5", "Starting tree vines"},
        {"Frame 6", "On log at end"},
        {"Frame 7", "Lash back to platform near start"},
        {"Frame 8", "Rope log by Frame 7"},
        {"Frame 9", "East lonely pplatform in 2nd section"},
        {"Frame 10", "Ladder behind first swinging trap"},
        {"Frame 11", "On netting by ledge in 3rd section"},
        {"Frame 12", "Ladder in 3rd section"},
        {"Frame 13", "Glide down from frame 12"},
        {"Frame 14", "Glide from top platform at rails"},
        {"Frame 15", "Swing across and drop down before rails"},
    },
    {
        // Steves
        {"Steve", "Just outside the entrance to the level"},
    },
    {
        // Frills
        {"Frill 1", "Outside entrance to level"},
        {"Frill 2", "Along pathway"},
    }
);

const LevelCollectibleInfo OverworldInfo(
    "Southern Rivers",
    {
        // Cogs
        {"Cog 1", "Underwater in central coral reef in Wobbygon Bay"},
        {"Cog 2", "Seaweed south of Warp Crystal Island in Wobbygon Bay"},
        {"Cog 3", "Wobbygon Bay Oil Rig Island"},
    },
    {
        // Bilbies
        {"Bilby 1", "Warp Crystal Island in Wobbygon Bay"},
        {"Bilby 2", "Back corner of Race Shack in Wobbygon Bay"},
        {"Bilby 3", "Underwater south of boathouse in Wobbygon Bay"},
    },
    {
        // Orbs
        {"Wobbygon Orb 1", "Between Race Shack and Warp Crystal Island in Wobbygon Bay"},
        {"Wobbygon Orb 2", "Underwater southwest corner in seaweed in Wobbygon Bay"},
        {"Sly's Shack Orb", "Spy egg button at Sly's Shack"},
        {"Dennis Freeway Orb", "Warp Crystal chain by Dennis Freeway mission"},
    },
    {
        // Picture Frames

    },
    {
        // Steves
        {"Steve - Freeway Training", "Beside the log that is the entrance to the freeway training grounds"},
        {"Steve - Wobbygon Bay", "Beneath bridge"},
        {"Steve - Sly's Shack", "Sly's Back Porch"},
    },
    {
        // Frills
        {"Dennis Freeway Frill", "Above crashed car at ledge under warp crystals by Dennis Freeway mission"},
        {"Dusty Burrows Frill", "In the Dusty Burrows Cul-De-Sac (by patchy boss fight)"},
        {"Lake Burramudgee Frill", "Sitting by the bridge at Lake Burramudgee"},
        {"Truck Tragedy Frill", "At bottom of hill where Truck Tragedy mission occurs"},
        {"Sheep Dip Frill", "Old Stony Creek (Where the Sheep Dip mission is located)"},
    }
);


const LevelCollectibleInfo AllInfo[] = {
    BuramudgeeInfo,
    OOInfo,
    SulphurRocksInfo,
    MBInfo,
	NNInfo,
    WetlandsInfo,
    FDInfo,
    FNFInfo,
	OverworldInfo
};