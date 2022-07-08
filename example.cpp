#include "encoder_factory.hpp"

int main() {
    // if (type == 1)
    //   return new SingleCharEncoder();
    // else if (type == 2)
    //   return new DoubleCharEncoder();
    // else if (type == 3)
    //   return new NGramEncoder(3);
    // else if (type == 4)
    //   return new NGramEncoder(4);
    // else if (type == 5)
    //   return new ALMEncoder(W);
    // else if (type == 6)
    //   return new ALMImprovedEncoder(W);
    // else
    //   return new DoubleCharEncoder();
    static const int kEncoderType = 6;
    static const char kKeyFilePath[] = "../datasets/words.txt";
    static const int kNumKeys = 234369;

    // load input key list
    // this input key list is already sorted
    std::vector<std::string> keys;
    int64_t total_key_len = 0;
    std::ifstream infile(kKeyFilePath);
    std::string key;
    int count = 0;
    while (infile.good() && count < kNumKeys) {
	infile >> key;
	keys.push_back(key);
	total_key_len += (key.length() * 8);
	count++;
    }

    // sample x% of the keys for building HOPE
    int skip = 100; // 1%
    // int skip = 10; // 10%
    // int skip = 1; // 100%
    std::vector<std::string> key_samples;
    for (int i = skip / 2; i < (int)keys.size(); i += skip) {
	key_samples.push_back(keys[i]);
    }

    // build the encoder
    hope::Encoder *encoder = hope::EncoderFactory::createEncoder(kEncoderType);
    encoder->build(key_samples, 5000); // 2nd para is dictionary size limit

    // compress all keys
    std::vector<std::string> enc_keys;
    for (int i = 0; i < (int)keys.size(); i++) {
        std::vector<int64_t> enc_list = encoder->my_encode(keys[i]);
        printf("uncomp_len %ld comp_len %ld: ", keys[i].length(), enc_list.size());
        for (auto token : enc_list) {
            printf("%ld ", token);
        }
        printf("\n"); // TODO: you can use fprintf to dump tokens to a file
	// int bit_len = encoder->encode(keys[i], buffer);
	// total_enc_len += bit_len;
	// enc_keys.push_back(std::string((const char *)buffer, (bit_len + 7) / 8));
    }

    
    return 0;
}
