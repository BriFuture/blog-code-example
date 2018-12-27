import $ from 'jquery'

export function fetchPost(url, params = {}) {
    return new Promise((resolve, reject) => {
        $.post(url, params).then( resp => {
            resolve( resp );
        }).catch( error => {
            reject( error );
        });
    });
}

export default {
    fetchPost: fetchPost
}
