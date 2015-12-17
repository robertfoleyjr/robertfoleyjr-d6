$Id: README.txt,v 1.2 2009/09/22 22:59:37 alexb Exp $

FEEDAPI TAGGER

Secondary parser for FeedAPI. Looks up mentions of vocabulary terms in title 
and descriptions of feeds and adds these terms to the feed array.

USAGE

1. Create vocabulary, populate with terms to be looked up.
2. Set up FeedAPI feed content type. Enable FeedAPI as secondary parser.
3. In "Look-up vocabulary" select box, choose the vocabulary created in step 1.

At this point FeedAPI Tagger is fully configured, but you won't see any tags 
created yet. Typically, you would want to use Feed Element Mapper to map the 
terms detected by FeedAPI tagger to vocabulary terms.

4. Install Feed Element mapper.
5. Go to node/id/map of one of your feeds.
5. Map tags to vocabulary created in 1.

