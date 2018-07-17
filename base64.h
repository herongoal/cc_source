#ifndef	_SVR_RTNS_BASE64_H_
#define	_SVR_RTNS_BASE64_H_


/**
 *	return	value:
 *	< 0 stands for insufficient buffer
 *	>= 0 stands for encoded result len
 */
int	encode_data(const void *data, size_t data_len, void *buff, size_t buff_len, bool padding);

/**
 *	return	value:
 *	< 0 stands for insufficient buffer
 *	>= 0 stands for encoded result len
 */
int	decode_data(const void *data, size_t data_len, void *buff, size_t buff_len);


#endif //_SVR_RTNS_BASE64_H_
