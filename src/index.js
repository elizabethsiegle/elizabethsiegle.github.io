import { getAssetFromKV } from '@cloudflare/kv-asset-handler'

addEventListener('fetch', event => {
  event.respondWith(handleRequest(event.request))
})

async function handleRequest(request) {
  const event = { request }
  try {
    // Use the static asset handler to serve files from the assets directory
    const response = await getAssetFromKV(event, {
      mapRequestToAsset: req => {
        // Handle root path
        if (req.url.endsWith('/')) {
          return new Request(req.url + 'index.html', req)
        }
        return req
      }
    })

    // Add cache headers
    const modifiedResponse = new Response(response.body, {
      status: response.status,
      statusText: response.statusText,
      headers: {
        ...response.headers,
        'Cache-Control': 'public, max-age=86400', // 24 hours
      },
    })

    return modifiedResponse
  } catch (e) {
    // If asset is not found, return 404
    return new Response('Not found', { status: 404 })
  }
}