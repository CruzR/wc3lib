/***************************************************************************
 *   Copyright (C) 2009 by Tamino Dauth                                    *
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

#include "mpqfile.hpp"
#include "mpq.hpp"

namespace wc3lib
{

namespace mpq
{

MpqFile::ListfileEntries MpqFile::listfileEntries(const string &content) throw (Exception)
{
	ListfileEntries result;
	string value(content);
	boost::algorithm::split(result, value, boost::algorithm::is_any_of(";\r\n"), boost::algorithm::token_compress_on);

	return result;
}

void MpqFile::removeData() throw (class Exception)
{
	if (!this->try_lock())
		throw Exception(boost::format(_("Unable to lock MPQ file \"%1%\".")) % path());

	this->block()->setFileSize(0);

	BOOST_FOREACH(SectorPtr sector, this->m_sectors)
		sector.reset();

	/// \todo Clear corresponding sector table in MPQ file?

	this->unlock();
}

std::streamsize MpqFile::readData(istream &istream, BOOST_SCOPED_ENUM(Sector::Compression) compression) throw (class Exception)
{
	removeData();

	if (!this->try_lock())
		throw Exception(boost::format(_("Unable to lock MPQ file \"%1%\".")) % path());

	std::streamsize bytes = 0;
	uint16 sectorSize = 0;

	if (!hasSectorOffsetTable())
	{
		if (this->block()->flags() & Block::Flags::IsSingleUnit)
		{
			SectorPtr sector(newSector());
			this->m_sectors.get<uint32>().insert(sector);
			sector->m_compression = compression;
			sector->m_sectorIndex = 0;
			sector->m_sectorOffset = 0;
			bytes = sector->readData(istream);
		}
		else
		{
			uint32 sectorIndex = 0;
			uint32 sectorOffset = 0;

			// split into equal sized (sometimes except last one) sectors
			while (!eof(istream))
			{
				// if last one it may be smaller than MPQ's sector size
				uint16 e = endPosition(istream)- istream.tellg();
				uint16 sectorSize = e >= this->m_mpq->sectorSize() ? this->m_mpq->sectorSize() : e;
				boost::scoped_array<byte> buffer(new byte[sectorSize]);
				std::streamsize sizeCounter = 0;
				wc3lib::read(istream, buffer[0], sizeCounter, sectorSize);

				SectorPtr sector(newSector());
				this->m_sectors.get<uint32>().insert(sector);
				sector->m_compression = compression;
				sector->m_sectorIndex = sectorIndex;
				sector->m_sectorOffset = sectorOffset;
				bytes += sector->readData(buffer.get(), sectorSize);

				++sectorIndex;
				sectorOffset += sectorSize;
			}
		}
	}
	// compressed
	else
	{
		/// \todo AHH!
	}

	this->unlock();

	return bytes;
}

std::streamsize MpqFile::appendData(istream &istream) throw (class Exception)
{
	throw Exception(_("MpqFile: appendData is not implemented yet!"));

	if (!this->try_lock())
		throw Exception(boost::format(_("Unable to lock MPQ file \"%1%\".")) % path());

	this->unlock();

	return 0;
}

std::streamsize MpqFile::sync() throw (class Exception)
{
	if (!this->try_lock())
		throw Exception(boost::format(_("Unable to lock MPQ file \"%1%\".")) % path());
	/*
	 * FIXME
	if (!this->mpq()->fileLock().is_locked())
		throw Exception(boost::format(_("Corresponding MPQ archive \"%1%\" of file \"%2%\" isn't locked.")) % mpq()->path() % path());
	*/

	ofstream ostream(mpq()->path(), std::ios_base::binary | std::ios_base::out);
	ostream.seekp(mpq()->startPosition());
	// TODO two operation or one? (for one std::streamoff must be at least uint64!!!
	ostream.seekp(boost::numeric_cast<std::streamoff>(block()->blockOffset()), std::ios_base::cur);
	ostream.seekp(boost::numeric_cast<std::streamoff>(block()->extendedBlockOffset()), std::ios_base::cur);
	//ostream.seekp(boost::numeric_cast<std::streamoff>(block()->largeOffset()), std::ios_base::cur);

	this->unlock();
}

std::streamsize MpqFile::writeData(ostream &ostream) const throw (class Exception)
{
	std::streamsize bytes = 0;

	BOOST_FOREACH(const SectorPtr &sector, this->sectors())
	{
		try
		{
			bytes += sector->writeData(ostream);
		}
		catch (Exception &exception)
		{
			throw Exception(boost::format(_("Sector error (sector %1%, file \"%2%\"):\n%3%")) % sector->sectorIndex() % this->path() % exception.what());
		}
	}

	return bytes;
}

MpqFile::ListfileEntries MpqFile::listfileEntries() const throw (Exception)
{
	std::basic_ostringstream<byte> stream;
	this->writeData(stream);

	return listfileEntries(stream.str());
}

MpqFile::MpqFile(class Mpq *mpq, class Hash *hash) : m_mpq(mpq), m_hash(hash), m_path("")
{
	if (hash != 0)
		hash->m_mpqFile = this;
}

MpqFile::~MpqFile()
{
}

class Sector* MpqFile::newSector() throw ()
{
	try
	{
		return new Sector(this);
	}
	catch (std::bad_alloc &)
	{
		return 0;
	}
}

std::streamsize MpqFile::read(istream &istream) throw (class Exception)
{
	// if we have a sector offset table and file is encrypted we first need to know its path for proper decryption!
	if (hasSectorOffsetTable() && isEncrypted() && path().empty())
	{
		istream.seekg(this->block()->blockSize(), std::ios_base::cur);

		return 0;
	}

	if (!this->try_lock())
		throw Exception(boost::format(_("Unable to lock MPQ file \"%1%\".")) % path());

	std::streamsize bytes = 0;

	// This table is not present if this information can be calculated.
	// If the file is not compressed/imploded, then the size and offset of all sectors is known, based on the archive's SectorSizeShift. If the file is stored as a single unit compressed/imploded, then the SectorOffsetTable is omitted, as the single file "sector" corresponds to BlockSize and FileSize, as mentioned previously.
	if  (!hasSectorOffsetTable())
	{
		//std::cout << "File is not imploded/compressed or it's a single unit " << std::endl;

		const uint16 sectorSize = (this->block()->flags() & Block::Flags::IsSingleUnit) ? this->block()->fileSize() : this->m_mpq->sectorSize();
		// If the file is stored as a single unit (indicated in the file's Flags), there is effectively only a single sector, which contains the entire file data.
		const uint32 sectors = (this->m_hash->block()->flags() & Block::Flags::IsSingleUnit) ? 1 : this->m_hash->block()->blockSize() / sectorSize;

		uint32 newOffset = 0;
		// not necessary for single units
		const uint32 lastSize = this->m_hash->block()->blockSize() % sectorSize;

		for (uint32 i = 0; i < sectors; ++i)
		{
			SectorPtr sector(newSector());
			sector->m_sectorIndex = i; // important for index function in sectors container
			sector->m_sectorOffset = newOffset;
			sector->m_sectorSize = sectorSize;
			this->m_sectors.insert(sector);
			//this->m_sectors.get<uint32>().insert(sector);

			newOffset += sector->sectorSize();
		}

		// the last sector may contain less than this, depending on the size of the entire file's data.
		if (!(this->m_hash->block()->flags() & Block::Flags::IsSingleUnit) && lastSize > 0)
		{
			SectorPtr sector(newSector());
			sector->m_sectorIndex = this->sectors().size(); // important for index function in sectors container
			sector->m_sectorOffset = newOffset;
			sector->m_sectorSize = lastSize;
			this->m_sectors.insert(sector);
		}
	}
	// However, the SectorOffsetTable will be present if the file is compressed/imploded and the file is not stored as a single unit, even if there is only a single sector in the file (the size of the file is less than or equal to the archive's sector size).
	// sector offset table
	else
	{
		// sector number calculation from StormLib
		// hf->nBlocks  = (hf->pBlock->dwFSize + ha->dwBlockSize - 1) / ha->dwBlockSize;

		const uint32 sectors = (this->block()->fileSize() + this->mpq()->sectorSize() - 1) / this->mpq()->sectorSize();

		// StormLib specific implementation (extra data)?!
		//dwToRead = (hf->nBlocks+1) * sizeof(DWORD);
		//if(hf->pBlock->dwFlags & MPQ_FILE_HAS_EXTRA)
			//dwToRead += sizeof(DWORD);

		// last offset contains file size
		boost::scoped_array<uint32> offsets(new uint32[sectors + 1]);
		wc3lib::read<uint32>(istream, offsets[0], bytes, (sectors + 1) * sizeof(uint32));

		// The SectorOffsetTable, if present, is encrypted using the key - 1.
		if (isEncrypted())
			DecryptData(Mpq::cryptTable(), offsets.get(), sizeof(uint32) * (sectors + 1), fileKey() - 1);

		for (uint32 i = 0; i < sectors; ++i)
		{
			SectorPtr sector(newSector());
			sector->m_sectorIndex = i; // important for index function in sectors container
			sector->m_sectorOffset = offsets[i];
			this->m_sectors.insert(sector);
		}

		// The last entry contains the file size, making it possible to easily calculate the size of any given sector.
		uint32 size = offsets[sectors];

		// calculate size of each sector
		BOOST_REVERSE_FOREACH(SectorPtr sector, this->m_sectors)
		{
			sector->m_sectorSize = size - sector->m_sectorOffset;
			//std::cout << "Sector size is " << sector->m_sectorSize << std::endl;
			size = sector->m_sectorOffset;
		}
	}

	this->unlock();

	return bytes;
}


std::streamsize MpqFile::write(ostream &ostream) const throw (class Exception)
{
	return 0;
}

void MpqFile::remove() throw (class Exception)
{
	if (!this->try_lock())
		throw Exception(boost::format(_("Unable to lock MPQ file \"%1%\".")) % path());

	this->m_hash->clear();

	/// @todo Clear file content -> usually unnecessary?
	//BOOST_FOREACH(class Sector *sector, this->m_sectors)
		//bytes += sector->clear();

	this->unlock();
}

bool MpqFile::rename(const std::string &newName, bool overwriteExisting) throw (class Exception)
{
	return this->move((this->path().has_parent_path() ? this->path().parent_path() / newName : newName).string(), overwriteExisting);
}

bool MpqFile::move(const boost::filesystem::path &newPath, bool overwriteExisting) throw (class Exception)
{
	if (this->m_path == newPath)
		return false;

	if (!this->try_lock())
		throw Exception(boost::format(_("Unable to lock MPQ file \"%1%\".")) % path());

	class MpqFile *mpqFile = this->m_mpq->findFile(newPath, this->locale(), this->platform());

	if (mpqFile != 0)
	{
		if (!overwriteExisting)
			return false;

		mpqFile->remove();
	}

	this->m_hash->changePath(newPath);

	this->unlock();

	return true;
}

class MpqFile& MpqFile::operator<<(const class MpqFile &mpqFile) throw (class Exception)
{
	arraystream stream;
	mpqFile.writeData(stream);
	this->appendData(stream);

	return *this;
}

class MpqFile& MpqFile::operator>>(class Mpq &mpq) throw (class Exception)
{
	mpq.addFile(*this, true, false);

	return *this;
}

class MpqFile& MpqFile::operator>>(class MpqFile &mpqFile) throw (class Exception)
{
	arraystream stream;
	this->writeData(stream);
	mpqFile.appendData(stream);

	return *this;
}

}

}
