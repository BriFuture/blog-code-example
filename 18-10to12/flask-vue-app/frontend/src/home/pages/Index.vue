<template>
  <div>
    <hr />
    This is Index Page In Home SPA.
    <form class="m-1">
      <div class="m-1">
        Username: <input type="text" v-model="username"/>
      </div>
      <div class="m-1">
        Password: <input type="password" v-model="password"/>
      </div>
      <div class="m-1">
        <button type="button" @click="toSignin()">SignIn</button>
      </div>
    </form>
  </div>
</template>

<script>
import {signin} from '../api.js'

export default {
  name: 'homeIndex',
  data() {
    return {
      username: null,
      password: null,
    }
  },
  methods: {
    toSignin: function() {
      signin({
        username: this.username,
        password: this.password
      }).then( resp => {
        if( resp.status === 'success' ) {
          window.location = '/user'
        } else {
          alert('Username or password is wrong.')
        }
      })
    }
  }
}
</script>

<style>
.m-1 {
  margin: 5px;
}
</style>

