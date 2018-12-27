import {fetchPost} from '../api.js'

export const signin = function(params) {
    return fetchPost('/api/home/signin', params);
}