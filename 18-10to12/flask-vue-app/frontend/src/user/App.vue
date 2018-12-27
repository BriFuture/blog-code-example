<template>
  <div id="app">
    <img alt="Vue logo" src="@/assets/logo.png">
    <div>Hello, this is User App</div>
    <router-view></router-view>
  </div>
</template>

<script>
export default {
  name: 'user',
}
</script>

<style>
#app {
  font-family: 'Avenir', Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}
</style>