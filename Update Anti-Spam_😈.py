import better_profanity
from better_profanity import profanity
from langdetect import detect_langs
from langdetect import DetectorFactory

client = MongoClient()
client = MongoClient(MONGO_DB_URI)
@@ -24,7 +25,7 @@

CMD_STARTERS = '/'
profanity.load_censor_words_from_file('./profanity_wordlist.txt')

DetectorFactory.seed = 0

async def can_change_info(message):
    result = await tbot(
@@ -420,7 +421,7 @@ async def del_profanity(event):
            if event.chat_id == c['id']:
                a = detect_langs(msg)
                for ele in a:
                 if str(ele).split(':')[0] == 'en':
                 if str(ele).split(':')[0] != 'en':
                    await event.delete()
                    if sender.username is None:
                        st = sender.first_name
