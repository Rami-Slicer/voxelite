#pragma once

#include <world.hpp>
#include <chunk.hpp>

#include <fstream>
#include <iostream>
#include <json.hpp>
#include <random>

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>




class WorldSerializer {
    public:
        WorldSerializer(std::string out_path);

        void exportWorld(World in);

    private:
        std::string path;

        // std::ofstream ofs;
        
        void add_chunk(VoxelChunk in);
};

class WorldDeserializer {
    public:
        WorldDeserializer(std::string in_path);
};