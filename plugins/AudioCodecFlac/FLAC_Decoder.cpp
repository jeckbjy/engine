#include "FLAC_Decoder.h"
#include "Cute/Stream.h"

CUTE_NS_BEGIN

//////////////////////////////////////////////////////////////////////////
// flac callback
//////////////////////////////////////////////////////////////////////////
FLAC__StreamDecoderReadStatus flac_read(const FLAC__StreamDecoder*, FLAC__byte buffer[], size_t* bytes, void* data)
{
	FLACDecoder* decoder = (FLACDecoder*)(data);
	Stream* stream = decoder->getStream();

	bool result = stream->read(buffer, *bytes);
	if (result)
		return FLAC__STREAM_DECODER_READ_STATUS_CONTINUE;
	else
		return FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM;
}

FLAC__StreamDecoderWriteStatus flac_write(const FLAC__StreamDecoder*, const FLAC__Frame* frame, const FLAC__int32* const buffer[], void* data)
{
	FLACDecoder* decoder = (FLACDecoder*)(data);
	Stream* stream = decoder->getStream();

	return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
}

FLAC__StreamDecoderSeekStatus flac_seek(const FLAC__StreamDecoder*, FLAC__uint64 offset, void* data)
{
	FLACDecoder* decoder = (FLACDecoder*)(data);
	Stream* stream = decoder->getStream();

	stream->seek(offset);
	return FLAC__STREAM_DECODER_SEEK_STATUS_OK;
}

FLAC__StreamDecoderTellStatus flac_tell(const FLAC__StreamDecoder*, FLAC__uint64* offset, void* data)
{
	FLACDecoder* decoder = (FLACDecoder*)(data);
	Stream* stream = decoder->getStream();

	*offset = stream->position();

	return FLAC__STREAM_DECODER_TELL_STATUS_OK;
}

FLAC__StreamDecoderLengthStatus flac_length(const FLAC__StreamDecoder*, FLAC__uint64* length, void* data)
{
	FLACDecoder* decoder = (FLACDecoder*)(data);
	Stream* stream = decoder->getStream();

	*length = stream->length();
	return FLAC__STREAM_DECODER_LENGTH_STATUS_OK;
}

FLAC__bool flac_eof(const FLAC__StreamDecoder*, void* data)
{
	FLACDecoder* decoder = (FLACDecoder*)(data);
	Stream* stream = decoder->getStream();

	return stream->eof();
}

void flac_meta(const FLAC__StreamDecoder*, const FLAC__StreamMetadata* meta, void* data)
{
	FLACDecoder* decoder = (FLACDecoder*)(data);

	if (meta->type == FLAC__METADATA_TYPE_STREAMINFO)
	{
		// …Ë÷√
	}
}

void flac_error(const FLAC__StreamDecoder*, FLAC__StreamDecoderErrorStatus, void* data)
{
	FLACDecoder* decoder = (FLACDecoder*)(data);
	// set error
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
FLACDecoder::FLACDecoder()
	: m_decoder(NULL)
	, m_stream(NULL)
{
}

FLACDecoder::~FLACDecoder()
{
	close();
}

bool FLACDecoder::open(const Stream* stream)
{
	m_decoder = FLAC__stream_decoder_new();
	if (m_decoder == NULL)
	{
		return false;
	}

	FLAC__stream_decoder_init_stream(
		m_decoder, 
		&flac_read,
		&flac_seek,
		&flac_tell,
		&flac_length,
		&flac_eof,
		&flac_write,
		&flac_meta,
		&flac_error,
		this);

	if (!FLAC__stream_decoder_process_until_end_of_metadata(m_decoder))
	{
		close();
		return false;
	}

	return true;
}

uint FLACDecoder::read(uint8* buffer, uint length)
{
	return 0;
}

void FLACDecoder::seek(uint offset)
{
	FLAC__stream_decoder_seek_absolute(m_decoder, offset);
}

void FLACDecoder::close()
{
	if (m_decoder != NULL)
	{
		FLAC__stream_decoder_finish(m_decoder);
		FLAC__stream_decoder_delete(m_decoder);
		m_decoder = NULL;
	}
}

CUTE_NS_END
