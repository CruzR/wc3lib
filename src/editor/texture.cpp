/***************************************************************************
 *   Copyright (C) 2011 by Tamino Dauth                                    *
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

#include <QFileInfo>
#include <QtCore>

#include <KTemporaryFile>

#include <Ogre.h>

#include "texture.hpp"
#include "mpqprioritylist.hpp"
#include "qblp/blpiohandler.hpp"
#include "Plugin_BlpCodec/blpcodec.hpp"

namespace wc3lib
{

namespace editor
{

Texture::Texture(const KUrl &url) : Resource(url, Type::Texture)
{
}

Texture::~Texture()
{
	m_ogreTexture.setNull();
}

void Texture::clearBlp()
{
	m_blp.reset();
}

void Texture::clearQt()
{
	m_qt.reset();
}

void Texture::clearOgre()
{
	m_ogre.reset();
}

void Texture::clearOgreTexture()
{
	m_ogreTexture.setNull();
}

void Texture::clear() throw ()
{
	clearBlp();
	clearQt();
	clearOgre();
	clearOgreTexture();
}


void Texture::loadBlp() throw (Exception)
{
	if (hasBlp())
		return;

	if (!hasQt()) /// TODO cannot convert from OGRE to BLP.
	{
		QString tmpFileName;

		if (!this->source()->download(url(), tmpFileName, 0))
			throw Exception(boost::format(_("Unable to download file from URL \"%1%\".")) % url().toLocalFile().toUtf8().constData());

		ifstream ifstream(tmpFileName.toUtf8().constData(), std::ios::binary | std::ios::in);

		if (!ifstream)
			throw Exception(boost::format(_("Unable to open temporary file \"%1%\".")) % tmpFileName.toUtf8().constData());

		blp::dword identifier;
		ifstream.read((blp::char8*)&identifier, sizeof(identifier));

		if (blp::Blp::hasFormat((blp::byte*)&identifier, sizeof(identifier)))
		{
			BlpPtr blpImage(new blp::Blp());

			ifstream.seekg(0); // jump to beginning of stream
			blpImage->read(ifstream);

			m_blp.swap(blpImage); // exception safe (won't change image if ->read throws exception
		}
		// if it's not BLP we need to convert it from Qt
		else
		{
			loadQt(); // if it doesn't throw it continues
			loadBlp();
		}
	}
	// if we have already an image it seems to be faster to read from it instead of the original file
	else
	{
		BlpIOHandler ioHandler;
		BlpPtr blpImage(new blp::Blp());

		if (!ioHandler.write(*qt().data(), blpImage.data()))
			throw Exception(_("Unable to convert Qt image into BLP."));

		blpImage.swap(m_blp); // exception safe (won't change image if handler has some error
	}
}

void Texture::loadQt() throw (Exception)
{
	if (hasQt())
		return;

	// if we have already an image it seems to be faster to read from it instead of the original file
	if (hasBlp())
	{
		BlpIOHandler ioHandler;
		QtPtr qtImage(new QImage());

		if (!ioHandler.read(qtImage.data(), *m_blp.data()))
			throw Exception(_("Unable to convert BLP image into Qt."));

		m_qt.swap(qtImage); // exception safe (won't change image if handler has some error
	}
	else if (hasOgre())
	{
		QtPtr qtImage(new QImage());

		// TODO do we have to use getPixelBox(), does it copy the whole buffer and where do we get its size from?
		if (!qtImage->loadFromData(ogre()->getData(), ogre()->getSize()))
			throw Exception(_("Unable to convert OGRE image into Qt."));

		m_qt.swap(qtImage); // exception safe (won't change image if handler has some error
	}
	else
	{
		QString tmpFileName;

		if (!this->source()->download(url(), tmpFileName, 0))
			throw Exception(boost::format(_("Unable to download file from URL \"%1%\".")) % url().toLocalFile().toUtf8().constData());

		QtPtr qtImage(new QImage());

		if (!qtImage->load(tmpFileName))
			throw Exception(boost::format(_("Unable to load Qt image from temporary file \"%1%\".")) % tmpFileName.toUtf8().constData());

		m_qt.swap(qtImage); // exception safe (won't change image if ->read throws exception
	}
}

void Texture::loadOgre() throw (Exception)
{
	if (hasOgre())
	{
		Q_ASSERT(m_ogre.data());

		return;
	}

	// if we have already an image it seems to be faster to read from it instead of the original file
	if (hasBlp())
	{
		OgrePtr ogreImage(new Ogre::Image());
		BlpCodec codec;
		Ogre::Codec::DecodeResult result(codec.decode(*m_blp.data()));
		BlpCodec::ImageData *imageData((BlpCodec::ImageData*)(result.second.get()));
		ogreImage->loadRawData((Ogre::DataStreamPtr&)result.first, imageData->width, imageData->height, imageData->depth, imageData->format, 1, boost::numeric_cast<std::size_t>(imageData->num_mipmaps));
		// TODO check correct loading state, exception handling?

		m_ogre.swap(ogreImage); // exception safe (won't change image if handler had some error)
	}
	// if we have already an image it seems to be faster to read from it instead of the original file
	else if (hasQt())
	{
		// TODO Isn't there any faster way of accessing Qt image's buffer data than saving it into a buffer object (member function etc.)? If direct access provides raw buffer use Ogre::Image::loadRawData() instead of Ogre::Image::load()!
		QBuffer buffer;
		qt()->save((QIODevice*)&buffer);
		Ogre::MemoryDataStreamPtr stream(new Ogre::MemoryDataStream(buffer.data().data(), buffer.size()));
		OgrePtr ogreImage(new Ogre::Image());
		ogreImage->load((Ogre::DataStreamPtr&)stream);
		//ogreImage->loadRawData(stream, qt()->width(), imageData->height, imageData->depth, imageData->format, 1, boost::numeric_cast<std::size_t>(imageData->num_mipmaps));

		m_ogre.swap(ogreImage); // exception safe (won't change image if handler had some error)
	}
	// read from file
	else
	{
		QString tmpFileName;

		if (!this->source()->download(url(), tmpFileName, 0))
			throw Exception(boost::format(_("Unable to download file from URL \"%1%\".")) % url().toLocalFile().toUtf8().constData());

		OgrePtr ogreImage(new Ogre::Image());
		const QString extension = QFileInfo(url().toLocalFile()).suffix(); // extension is not necessary if header contains information - Ogre::Image::load()

		try
		{
			std::ifstream ifs(tmpFileName.toUtf8().constData(), std::ios::binary | std::ios::in);

			if (!ifs)
				throw Exception(boost::format(_("Unable to open tempory file \"%1%\".")) % tmpFileName.toUtf8().constData());

			Ogre::DataStreamPtr dataStream(new Ogre::FileStreamDataStream(tmpFileName.toUtf8().constData(), &ifs, false));
			ogreImage->load(dataStream, extension.toUtf8().constData());
			// TODO check correct loading state, exception handling?

			ifs.close();
		}
		catch (Ogre::Exception &exception)
		{
			throw Exception(exception.getFullDescription());
		}

		m_ogre.swap(ogreImage); // exception safe (won't change image if ->read throws exception
	}

	Q_ASSERT(m_ogre.data());
}

void Texture::loadOgreTexture() throw (Exception)
{
	const bool hasOgre = this->hasOgre();

	if (!hasOgre)
		loadOgre();

	Ogre::TexturePtr tex;

	try
	{
		// TODO use custom root
		tex = Ogre::Root::getSingleton().getTextureManager()->create(Ogre::String(QFileInfo(url().toLocalFile()).baseName().toUtf8().constData()), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		tex->loadImage(*ogre());
	}
	catch (Ogre::Exception &exception)
	{
		if (!hasOgre)
			clearOgre();

		throw Exception(exception.getFullDescription());
	}

	m_ogreTexture = tex;

	if (!hasOgre)
		clearOgre();
}

void Texture::loadAll() throw (Exception)
{
	loadBlp();
	loadQt();
	loadOgre();
	loadOgreTexture();
}

void Texture::reload() throw (Exception)
{
	bool hasBlp = this->hasBlp();
	bool hasQt = this->hasQt();
	bool hasOgre = this->hasOgre();
	bool hasOgreTexture = this->hasOgreTexture();
	clear();

	if (hasBlp)
		loadBlp();

	if (hasQt)
		loadQt();

	if (hasOgre)
		loadOgre();

	if (hasOgreTexture)
		loadOgreTexture();
}

namespace
{

inline QString compressionOption(const QStringList &list, const QString key)
{
	const int index = list.indexOf(key);

	if (index == -1)
		return "";

	const int charIndex = list[index].indexOf('=');

	if (charIndex == -1 || charIndex == list[index].length() - 1)
		return "";

	return list[index].mid(charIndex + 1);
}

}

void Texture::save(const KUrl &url, const QString &format, const QString &compression) const throw (Exception)
{
	KTemporaryFile tmpFile;

	if (!tmpFile.open())
		throw Exception(boost::format(_("Temporary file \"%1%\" cannot be opened.")) % tmpFile.fileName().toUtf8().constData());

	QString realFormat = format;

	if (realFormat.isEmpty())
	{
		realFormat = QFileInfo(url.toLocalFile()).suffix();

		if (format.isEmpty())
			throw Exception(_("Unknown format!"));
	}

	// get all compression options
	const QStringList compressionOptions = compression.split(":");
	int quality = realFormat == "blp" ? blp::Blp::defaultQuality : -1;
	QString qualityString = compressionOption(compressionOptions, "Quality");
	std::size_t mipMaps = realFormat == "blp" ? blp::Blp::defaultMipMaps : 1;
	QString mipMapsString = compressionOption(compressionOptions, "MipMaps");
	bool threads = realFormat == "blp" ? blp::Blp::defaultThreads : true;
	QString threadsString = compressionOption(compressionOptions, "Threads");

	bool ok = false;
	int tmpValue = qualityString.toInt(&ok);

	if (ok && tmpValue >= -1 && tmpValue <= 100)
		quality = tmpValue;

	tmpValue = mipMapsString.toInt(&ok);

	if (tmpValue >= 1 && tmpValue <= blp::Blp::maxMipMaps)
		mipMaps = tmpValue;

	if (!threadsString.isEmpty())
		threads = threadsString == "1" || threadsString == "true" || threadsString == "TRUE";

	if (realFormat == "blp" && hasBlp())
	{
		ofstream ofstream(tmpFile.fileName().toUtf8().constData(), std::ios::binary | std::ios::out);
		blp()->write(ofstream, quality, mipMaps, threads);
	}
	else if (hasQt())
	{
		qt()->save(&tmpFile, realFormat.toUtf8().constData(), quality);
	}
	/// TODO we cannot convert OGRE images (write into stream etc.)
	else
	{
		throw Exception(boost::format(_("Temporary file \"%1%\" cannot be converted by using an OGRE image: Not implemented yet!")) % tmpFile.fileName().toUtf8().constData());
	}

	if  (!this->source()->upload(tmpFile.fileName(), url, 0))
		throw Exception(boost::format(_("Unable to upload temporary file \"%1%\" to URL \"%2%\"")) % tmpFile.fileName().toUtf8().constData() % url.toEncoded().constData());
}

}

}
