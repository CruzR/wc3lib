/***************************************************************************
 *   Copyright (C) 2010 by Tamino Dauth                                    *
 *   tamino@cdauth.eu                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef WC3LIB_W3G_PLATFORM_HPP
#define WC3LIB_W3G_PLATFORM_HPP

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/zlib.hpp>

#include "../core.hpp"

namespace wc3lib
{

namespace w3g
{

typedef uint8_t byte;
typedef uint32_t dword;
typedef uint16_t word;
typedef boost::iostreams::stream<boost::iostreams::basic_array<byte> > arraystream;
typedef boost::iostreams::stream<boost::iostreams::basic_array_source<byte> > iarraystream;
typedef boost::iostreams::stream<boost::iostreams::basic_array_sink<byte> > oarraystream;

struct Header
{
	byte identifier[28]; // Warcraft III recorded game\0x1A\0
	dword size;
	dword fileSize;
	dword fileVersion; // 0x00 for WarCraft III with patch <= 1.06, 0x01 for WarCraft III patch >= 1.07 and TFT replays

	dword decompressedSize; // excluding header
	dword dataBlocks; // number
};

struct SubHeader0
{
	word unknown;
	word version;
	word build;
	word flags;
	dword length; // replay length in msec
	dword checksum;
};

struct SubHeader1
{
	dword identifier;
	dword version;
	dword build;
	word flags;
	dword length; // replay length in msec
	dword checksum;
};

struct DataBlockHeader
{
	word compressedSize;
	word size;
	dword unknown; // probably checksum
};

struct PlayerRecord
{
};

struct GameName
{
};

struct MapSettings
{
};

struct MapRecord
{
};

struct MapAndCreatorName
{
};

BOOST_SCOPED_ENUM_START(VersionInformationEntry)
{
	GameVersion,
	ReplayVersion,
	War3Version,
	ReleaseData,
	MaxVersionInformationEntries
};
BOOST_SCOPED_ENUM_END

// this table should be used by class W3g to get some information about the replay

const char *versionInformation[][MaxVersionInformationEntries] =
{
	{ "1.00",               "1.00.4448",            "1.0. 0.4448",          "2002-07-03" },
	{ "1.01",               "1.01.4482",            "1.0. 1.4482",          "2002-07-05" },
	{ "1.01b",              "1.01.4482",            "1.0. 1.4483",          "2002-07-10" },
	{ "1.01c",              "1.01.4482",            "?",                    "2002-07-28" },
	{ "1.02",               "1.02.4531",            "1.0. 1.4531",          "2002-08-15" },
	{ "1.02a",              "1.02.4531",            "1.0. 1.4563",          "2002-09-06" },
	{ "1.03",               "1.03.4572",            "1.0. 3.4653",          "2002-10-09" },
	{ "1.04",               "1.04.4654",            "1.0. 3.4709",          "2002-11-04" },
	{ "1.04b",              "1.04.4654",            "1.0. 3.4709",          "2002-11-07" },
	{ "1.04c",              "1.04.4654",            "1.0. 4.4905",          "2003-01-30" },
	{ "1.05",               "1.05.4654",            "1.0. 5.4944",          "2003-01-30" },
	{ "1.06",               "1.06.4656",            "1.0. 6.5551",          "2003-06-03" },
	{ "1.07",               "1.07.6031",            "1.0. 7.5535",          "2003-07-01" },
	{ "1.10",               "1.10.6034",            "1.0.10.5610",          "2003-06-30" },
	{ "1.11",               "1.11.6035",            "1.0.11.5616",          "2003-07-15" },
	{ "1.12",               "1.12.6036",            "1.0.12.5636",          "2003-07-31" },
	{ "1.13",               "1.13.6037",            "1.0.13.5816",          "2003-12-16" },
	{ "1.13b",              "1.13.6037",            "1.0.13.5818",          "2003-12-19" },
	{ "1.14",               "1.14.6039",            "1.0.13.5840",          "2004-01-07" },
	{ "1.14b",              "1.14.6040",            "1.0.13.5840",          "2004-01-10" },
	{ "1.15",               "1.15.6043",            "1.0.15.5926",          "2004-05-10" },
	{ "1.16",               "1.16.6046",            "1.0.16.5926",          "2004-07-01" },
	{ "1.17",               "1.17.6050",            "1.0.16.5988",          "2004-09-20" }
};

const char *versionInformationBeta[][MaxVersionInformationEntries] =
{
	{ "301",                "version 0",            "6010",                 "2003-03-04" },
	{ "302",                "version 0",            "6011",                 "2003-03-11" },
	{ "303",                "version 0",            "6012",                 "2003-03-14" },
	{ "304",                "version 0",            "6013",                 "2003-03-19" },
	{ "304a",               "version 0",            "6013",                 "2003-03-24" },
	{ "305",                "version 1(*)",         "6015",                 "2003-03-28" },
	{ "305a",               "version 1(*)",         "6016",                 "2003-03-30" },
	{ "306",                "version 1",            "6018",                 "2003-04-08" },
	{ "307",                "version 1",            "6019",                 "2003-04-11" },
	{ "308",                "version 1",            "6021",                 "2003-04-16" },
	{ "309",                "version 1",            "6022",                 "2003-04-17" },
	{ "310",                "version 1",            "6023",                 "2003-04-24" },
	{ "311",                "version 1",            "6027",                 "2003-04-30" },
	{ "312",                "version 1",            "6030",                 "2003-05-13" },
	{ "313",                "version 1",            "6031",                 "2003-05-19" },
	{ "314",                "version 1",            "6034",                 "2003-05-30" },
	{ "314a",               "version 1",            "6034",                 "2003-06-02" },
	{ "315",                "version 1",            "6034",                 "2003-06-10" }
};

/*
ushort "m": compressed size of the block
ushort original size: size of the original uncomprssed input data,
must be multiple of 2048, rest is filled with 0x00 bytes
uint hash: checksum over the block header and the block data,
the formula how this is computed won't be released to the public,
however reasonable requests might be answered

Now there are "m" bytes zlib compressed data, which can be decompressed/compressed with a zlib library.
Depending on the zlib implementation you might have to compute the zlib header by yourself. That would be:

byte[2] zlib header: can be skipped when reading, set to 0x78 and 0x01 when writing
byte[m - 2] deflate stream, use deflate stream implementation to decompress/compress

After uncompressing all blocks and appending them to each other, you have the original uncompressed file.
Depending on the type of file, the replay, gamecache or savegame file specifications will now apply.
*/
void readBlock(const byte *buffer, std::size_t bufferSize, byte *outputBuffer, std::size_t &outputSize)
{
	DataBlockHeader header;
	memcpy(&header, buffer, sizeof(header);
	iarraystream istream(&buffer[sizeof(header)], header.compressedSize);
	boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
	in.push(boost::iostreams::basic_zlib_decompressor<std::allocator<byte> >());
	in.push(istream);
	arraystream ostream;
	outputSize = boost::iostreams::copy(in, ostream);
	outputBuffer = new byte[outputSize];
	ostream.read(outputBuffer, outputSize);
}

void writeBlock(const byte *buffer, std::size_t bufferSize, byte *outputBuffer, std::size_t &outputSize)
{
	iarraystream istream(buffer, bufferSize);
	boost::iostreams::filtering_streambuf<boost::iostreams::output> out;
	out.push(boost::iostreams::basic_zlib_compressor<std::allocator<byte> >());
	out.push(istream);
	arraystream ostream;
	outputSize = boost::iostreams::copy(out, ostream);
	outputBuffer = new byte[outputSize + sizeof(DataBlockHeader)];
	DataBlockHeader.size = bufferSize;
	DataBlockHeader.compressedSize = outputSize;
	DataBlockHeader.unknown = 0;
	ostream.read(&outputBuffer[sizeof(DataBlockHeader)], outputSize);
}

}

}

#endif
