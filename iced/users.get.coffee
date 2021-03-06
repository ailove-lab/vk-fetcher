req = require 'request'
readline = require 'readline'
parser = require './parser'

rl = readline.createInterface
  input   : process.stdin
  output  : process.stdout
  terminal: false

queue = []
uids = ""
max_length = 4000

cnt = 0

rl.on 'line', (uid)->
    uids += uid
    if uids.length > max_length or cnt > 800
        queue.push uids
        uids = ""
        cnt = 0 
    else
        uids += ","
        cnt += 1
        
rl.on 'close', ->
    queue.push uids
    start_fetchers()

start_fetchers = ->
    fetch() for i in [0...100]

fetch = ()->

    uids = queue.shift()
    return unless uids?

    console.error queue.length
    await grab_url uids, defer err, uids, response
    unless err?
        for u in response
            {uid, sex, bdate, country} = u
            sex?=""
            bdate?=""
            country?=""
            console.log "#{uid}\t#{sex}\t#{bdate}\t#{country}"
    else
        console.error "ERROR"
        queue.push uids
    fetch()

grab_url = (uids, cb)->

    fields = "bdate,sex,country"
    url = "https://api.vk.com/method/users.get?v=3&user_ids=#{uids}&fields=#{fields}"
    await req.get url, defer err, res, data

    return cb err, uids if err
    return cb res.statusCode, uids unless res.statusCode is 200
    json = JSON.parse data
    return cb json.error, uids if json.error?
    cb null, uids, json.response
