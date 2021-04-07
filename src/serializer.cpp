#include <serializer.hpp>

std::string base64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// use bzip

WorldSerializer::WorldSerializer(std::string out_path) {
    this->path = out_path;
    // this->ofs = std::ofstream(this->path);

}

void WorldSerializer::exportWorld(World in) {
    std::vector<VoxelChunk> chunks = in.getChunks();

    boost::iostreams::filtering_streambuf<boost::iostreams::output> outbuf;
    outbuf.push(boost::iostreams::gzip_compressor());
    

    for(VoxelChunk &chunk : chunks) {

        std::string chunk_path = this->path;
        chunk_path += std::to_string(chunk.getChunkPos().x) + "-";
        chunk_path += std::to_string(chunk.getChunkPos().y) + "-";
        chunk_path += std::to_string(chunk.getChunkPos().z) + ".world";

        std::ofstream ofs_chunk(chunk_path);
        
        

        for(int i = 1024; i > 0;) {
            int randi = rand() % 16;
            char c = base64[rand() % 63];
            for(int j = 0; j < randi; j++) {
                i--;
                ofs_chunk << c;
            }
            
        }

    }
    boost::iostreams::close(outbuf); // Don't forget this!
}