#include <iostream>
#include <fstream>
#include <bitset>

// INFO about file found at 
// https://wiki.openstreetmap.org/wiki/SL2

int MAX_BUFFER_SIZE = (1024 * 1000);

// Header absolute positions
int FORMAT_POSITION = 0;
int BLOCKSIZE_POSITION = 4;
int LOG_POSITION = 8;

// Log relative positions
int CHANNEL_POSITION = 32;
int PACKET_SIZE_POSITION = 34;
int INDEX_POSITION = 36;
int LIMIT_POSITION = 40;
int FREQ_POSITION = 53;
int DEPTH_POSITION = 64;
int EXTA_INFO_POSITION = 100;
int TIME_POSITION = 140;

double POLAR_EARTH_RADIUS = 6356752.3142;

// Gets uint8 at specific position in byte stream
uint8_t getUint8(std::ifstream& stream, int position, int way, bool readonly=false){
    uint8_t output;
    if (!readonly){
        switch (way)
        {
            case 0:
                stream.seekg(position, std::ios_base::beg); // go to start position
                break;

            case 1:
                stream.seekg(position, std::ios_base::cur); // go to current position
                break;

            case 2:
                stream.seekg(position, std::ios_base::end); // go to end position
                break;
        
            default:
                break;
        }
    }

    
    stream.read((char *)&output, sizeof(output)); // assign pointer to output
    return output;
}

// Gets uint16 at specific position in byte stream
uint16_t getUint16(std::ifstream& stream, int position, int way, bool readonly=false){
    uint16_t output;
    if (!readonly){
        switch (way)
        {
            case 0:
                stream.seekg(position, std::ios_base::beg); // go to start position
                break;

            case 1:
                stream.seekg(position, std::ios_base::cur); // go to current position
                break;

            case 2:
                stream.seekg(position, std::ios_base::end); // go to end position
                break;
        
            default:
                break;
        }
    }

    
    stream.read((char *)&output, sizeof(output)); // assign pointer to output
    return output;
}

// Gets uint32 at specific position in byte stream
uint32_t getUint32(std::ifstream& stream, int position, int way, bool readonly=false){
    uint32_t output;
    if (!readonly){
        switch (way)
        {
            case 0:
                stream.seekg(position, std::ios_base::beg); // go to start position
                break;

            case 1:
                stream.seekg(position, std::ios_base::cur); // go to current position
                break;

            case 2:
                stream.seekg(position, std::ios_base::end); // go to end position
                break;
        
            default:
                break;
        }
    }

    
    stream.read((char *)&output, sizeof(output)); // assign pointer to output
    return output;
}

// Gets float at specific position in byte stream
float getFloat(std::ifstream& stream, int position, int way, bool readonly=false){
    float output;
    if (!readonly){
        switch (way)
        {
            case 0:
                stream.seekg(position, std::ios_base::beg); // go to start position
                break;

            case 1:
                stream.seekg(position, std::ios_base::cur); // go to current position
                break;

            case 2:
                stream.seekg(position, std::ios_base::end); // go to end position
                break;
        
            default:
                break;
        }
    }
    
    stream.read((char *)&output, sizeof(output)); // assign pointer to output
    return output;
}

// void writeCSV(std::ofstream& outCSV, ) {
//     outCSV << "Lat" << ",";
//     outCSV << "Long" << ",";
//     outCSV << "Depth" << ",";
//     outCSV << "Speed" << ",";
//     outCSV << "Temp" << ",";
//     outCSV << "Altitude" << ",";
//     outCSV << "Heading" << "\n";
// }

int readFile(const char *inPath, const char *outPath){
    
    // Print the input file 
    std::cout << "\nInput File: " << inPath << "\n";
    std::cout << "Output File: " << outPath << "\n\n";
    
    // Write to CSV
    std::ofstream outCSV;
    outCSV.open(outPath);
    
    // CSV Header
    outCSV << "latEnc" << ",";
    outCSV << "lngEnc" << ",";
    outCSV << "waterDepth" << ",";
    outCSV << "speedGps" << ",";
    outCSV << "temperature" << ",";
    outCSV << "altitude" << ",";
    outCSV << "heading" << ",";
    outCSV << "channel" << ",";
    outCSV << "frameIndex" << ",";
    outCSV << "upperLimit" << ",";
    outCSV << "lowerLimit" << ",";
    outCSV << "freq" << ",";
    outCSV << "keelDepth" << ",";
    outCSV << "speedWater" << ",";
    outCSV << "track" << ",";
    outCSV << "trackValid" << ",";
    outCSV << "waterSpeedValid" << ",";
    outCSV << "positionValid" << ",";
    outCSV << "waterTempValid" << ",";
    outCSV << "speedValid" << ",";
    outCSV << "altitudeValid" << ",";
    outCSV << "headingValid" << ",";
    outCSV << "\n";

    char buffer[MAX_BUFFER_SIZE]; // Create empty buffer
    
    // init file stream, set buffer and read as binary
    std::ifstream in;
    in.rdbuf()->pubsetbuf(buffer, sizeof(buffer)); // set buffer
    in.open(inPath, std::ifstream::in | std::ios::binary); // open file as binary
    
    // FORMAT
    uint16_t format = getUint16(in, FORMAT_POSITION, 0);    
    // Fails if not the correct format
    if (format != 2) {
        return 1;
    }
    
    // BLOCKSIZE
    uint16_t blockSize = getUint16(in, BLOCKSIZE_POSITION, 0);
    // Fails if not the correct block size
    if (blockSize != 3200 && blockSize != 1970){
        return 1;
    }

    // LOGS
    in.seekg(LOG_POSITION, std::ios_base::beg); // go to LOG_POSITION
    
    // Runs for every log sample point
    while (in.tellg() != -1) {
        std::streampos streamPos = in.tellg(); // set position of ifstream  

        // CHANNEL
        in.seekg(streamPos);
        uint16_t channel = getUint16(in, CHANNEL_POSITION, 1);

        // PACKET SIZE
        in.seekg(streamPos);
        uint16_t packetSize = getUint16(in, PACKET_SIZE_POSITION, 1);

        // FRAME INDEX
        in.seekg(streamPos);
        uint16_t frameIndex = getUint16(in, INDEX_POSITION, 1);

        // LIMITS
        in.seekg(streamPos);
        float upperLimit = getFloat(in, LIMIT_POSITION, 1);
        float lowerLimit = getFloat(in, LIMIT_POSITION, 1, true);

        // FREQ
        in.seekg(streamPos);
        uint8_t freq = getUint8(in, FREQ_POSITION, 1);

        // DEPTHS
        in.seekg(streamPos);
        float waterDepth = getFloat(in, DEPTH_POSITION, 1);
        float keelDepth = getFloat(in, DEPTH_POSITION, 1, true);

        // OTHERS (lat long set to WGS84 POLAR)
        in.seekg(streamPos);
        std::bitset<16> flags;
        float speedGps = getFloat(in, EXTA_INFO_POSITION, 1);
        float temperature = getFloat(in, EXTA_INFO_POSITION, 1, true);
        uint32_t lngEnc = getUint32(in, EXTA_INFO_POSITION, 1, true);
        uint32_t latEnc = getUint32(in, EXTA_INFO_POSITION, 1, true);
        float speedWater = getFloat(in, EXTA_INFO_POSITION, 1, true);
        float track = getFloat(in, EXTA_INFO_POSITION, 1, true);
        float altitude = getFloat(in, EXTA_INFO_POSITION, 1, true);
        float heading = getFloat(in, EXTA_INFO_POSITION, 1, true);
        in.read((char *)&flags, sizeof(flags));

        // Is Valid
        bool trackValid = flags.test(0);
        bool waterSpeedValid = flags.test(1);
        bool positionValid = flags.test(3);
        bool waterTempValid = flags.test(5);
        bool speedValid = flags.test(6);
        bool altitudeValid = flags.test(14);
        bool headingValid = flags.test(15);

        // TIME OFFSET
        in.seekg(streamPos);
        uint32_t timeOffset = getUint32(in, TIME_POSITION, 1);

        // END OF PACKET
        in.seekg(streamPos); 
        in.seekg(144, std::ios_base::cur);
        in.seekg(packetSize, std::ios_base::cur); 


        // PRINTING      
        // std::cout << "lat: " << latEnc << " long: " << lngEnc;
        // std::cout << " Depth: " << waterDepth << "m";
        // std::cout << " Speed: " << speedGps << "knots";
        // std::cout << " Temp: " << temperature << "c";
        // std::cout << " Altitude: " << altitude << "m";
        // std::cout << " Heading: " << heading;
        // std::cout << " altitudeValid: " << altitudeValid;
        // std::cout << std::endl;

        
        // STREAM CSV
        outCSV << latEnc << ",";
        outCSV << lngEnc << ",";
        outCSV << waterDepth << ",";
        outCSV << speedGps << ",";
        outCSV << temperature << ",";
        outCSV << altitude << ",";
        outCSV << heading << ",";
        outCSV << channel << ",";
        outCSV << frameIndex << ",";
        outCSV << upperLimit << ",";
        outCSV << lowerLimit << ",";
        outCSV << freq << ",";
        outCSV << keelDepth << ",";
        outCSV << speedWater << ",";
        outCSV << track << ",";
        outCSV << trackValid << ",";
        outCSV << waterSpeedValid << ",";
        outCSV << positionValid << ",";
        outCSV << waterTempValid << ",";
        outCSV << speedValid << ",";
        outCSV << altitudeValid << ",";
        outCSV << headingValid << ",";
        outCSV << "\n";


    }

    outCSV.close();
    in.close();

    return 0;
}


int main(int argc, const char *argv[]){
    
    return readFile(argv[1], argv[2]);       
}